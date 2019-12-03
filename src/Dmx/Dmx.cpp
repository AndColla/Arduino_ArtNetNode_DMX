#include "Dmx.h"

#include <inttypes.h>
#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <util/delay.h>

//********************************************//
//                 DMX_Buffer                 //
//********************************************//

DMX_Buffer::DMX_Buffer ( uint16_t buffer_size ) {
    m_refcount = (uint8_t*) malloc ( sizeof ( uint8_t ) );

    if ( buffer_size >= DMX_MIN_FRAME_SIZE && buffer_size <= DMX_MAX_FRAME_SIZE ) {
        m_buffer = (uint8_t*) malloc ( buffer_size );
        if ( m_buffer != NULL ) {
            memset ( (void *)m_buffer, 0x0, buffer_size );
            m_bufferSize = buffer_size;
        } else {
            m_buffer = 0x0;
        }
    } else {
        m_bufferSize = 0x0;
    }

    *m_refcount++;
}

DMX_Buffer::DMX_Buffer ( DMX_Buffer &buffer ) {
    // Copy references and make sure the parent object does not dispose our
    // buffer when deleted and we are still active
    this->m_refcount = buffer.m_refcount;
    (*this->m_refcount)++;
    
    this->m_buffer = buffer.m_buffer;
    this->m_bufferSize = buffer.m_bufferSize;
}

DMX_Buffer::~DMX_Buffer ( void ) {
    // If we are the last object using the allocated buffer
    // then free it together with the refcounter
    if ( --(*m_refcount) == 0 ) {
        if ( m_buffer ) {
            free ( m_buffer );
        }
        free ( m_refcount );
    }
}

uint16_t DMX_Buffer::getBufferSize ( void ) {
    return m_bufferSize;
}

uint8_t DMX_Buffer::getSlotValue ( uint16_t index ) {
    if (index < m_bufferSize) {
        return m_buffer[index];
    } else {
        return 0x0;
    }
}

void DMX_Buffer::setSlotValue ( uint16_t index, uint8_t value ) {
    if ( index < m_bufferSize ) {
        m_buffer[index] = value;
    }
}

void DMX_Buffer::setSlotRange ( uint16_t start, uint16_t end, uint8_t value ) {
    if ( start < m_bufferSize && end < m_bufferSize && start < end ) {
        memset ( (void *) &m_buffer[start], value, end-start+1 );
    }
}

void DMX_Buffer::clear ( void ) {
    memset ( (void *) m_buffer, 0x0, m_bufferSize );
}

uint8_t *DMX_Buffer::getPointer ( void ) {
    return m_buffer + 1;
}

uint8_t &DMX_Buffer::operator[] ( uint16_t index ) {
    return m_buffer[index];
}

//********************************************//
//                 DMX_Master                 //
//********************************************//

DMX_Master::DMX_Master ( int uart, uint16_t maxChannel, int controlPin )
: m_frameBuffer ( maxChannel + DMX_STARTCODE_SIZE ),
  m_uart ( uart ),
  m_controlPin ( controlPin )
{
    setStartCode ( DMX_START_CODE );
    pinMode ( m_controlPin, OUTPUT );
    setISRMode ( isr::Disabled );
}

DMX_Buffer &DMX_Master::getBuffer ( void )
{
    return m_frameBuffer;                               // Return reference to frame buffer
}

void DMX_Master::setStartCode ( uint8_t value )
{
    m_frameBuffer[0] = value;                           // Set the first byte in our frame buffer
}

void DMX_Master::setChannelValue ( uint16_t channel, uint8_t value )
{
    if ( channel > 0 ) {                                // Prevent overwriting the start code
        m_frameBuffer.setSlotValue ( channel, value );
    }
}

void DMX_Master::setChannelRange ( uint16_t start, uint16_t end, uint8_t value )
{
    if ( start > 0 )                                    // Prevent overwriting the start code
        m_frameBuffer.setSlotRange ( start, end, value );
}

void DMX_Master::enable  ( void )
{
    setISRMode ( isr::DMXTransmit );
}

void DMX_Master::disable ( void )
{
    setISRMode ( isr::Disabled );
}

isr::isrState DMX_Master::getISRState ( void )
{
    return m_isrState;
}

void DMX_Master::setISRState ( isr::isrState state )
{
    m_isrState = state;
}

void DMX_Master::setISRMode ( isr::isrMode mode )
{
    uint8_t controlPinStatus;

    switch ( m_uart ) {
        case 0:
            UCSR0C = (3<<UCSZ00)|(1<<USBS0);
            switch ( mode )
            {
                case isr::Disabled:
                    UCSR0B = 0x0;
                    controlPinStatus = LOW;
                    break;

                case isr::DMXTransmit:
                    UDR0 = 0x0;
                    controlPinStatus = HIGH;
                    m_isrState = isr::DmxBreak; 
                    UCSR0B = (1<<TXEN0) | (1<<TXCIE0);
                    break;
            }
            break;
        case 1:
            UCSR1C = (3<<UCSZ10)|(1<<USBS1);
            switch ( mode )
            {
                case isr::Disabled:
                    UCSR1B = 0x0;
                    controlPinStatus = LOW;
                    break;

                case isr::DMXTransmit:
                    UDR1 = 0x0;
                    controlPinStatus = HIGH;
                    m_isrState = isr::DmxBreak; 
                    UCSR1B = (1<<TXEN1) | (1<<TXCIE1);
                    break;
            }
            break;
        case 2:
            UCSR2C = (3<<UCSZ20)|(1<<USBS2);
            break;
        case 3:
            UCSR3C = (3<<UCSZ30)|(1<<USBS3);
            break;
    }

    digitalWrite ( m_controlPin, controlPinStatus );

}

ISR(USART0_TX_vect)
{
    static uint16_t			current_slot_0;

	switch ( DMX_Master_0.getISRState() )
	{
	case isr::DmxBreak:
		UCSR0A = 0x0;
        UBRR0H = (unsigned char)(((F_CPU + DMX_BREAK_RATE * 8L) / (DMX_BREAK_RATE * 16L) - 1)>>8);
        UBRR0L = (unsigned char) ((F_CPU + DMX_BREAK_RATE * 8L) / (DMX_BREAK_RATE * 16L) - 1);
        UDR0 = 0x0;
        
        if ( DMX_Master_0.getISRState() ==  isr::DmxBreak ) {
            DMX_Master_0.setISRState(isr::DmxStartByte);
        }
        
        break;

	case isr::DmxStartByte:
		UCSR0A = 0x0;
        UBRR0H = (unsigned char)(((F_CPU + DMX_BAUD_RATE * 8L) / (DMX_BAUD_RATE * 16L) - 1)>>8);
		UBRR0L = (unsigned char) ((F_CPU + DMX_BAUD_RATE * 8L) / (DMX_BAUD_RATE * 16L) - 1);

        current_slot_0 = 0;	
        UDR0 = DMX_Master_0.getBuffer()[ current_slot_0++ ];
        DMX_Master_0.setISRState(isr::DmxTransmitData);
		break;
	

	case isr::DmxTransmitData:
        // NOTE: we always send full frames of 513 bytes, this will bring us 
        // close to 40 frames / sec with no interslot delays
        #ifdef DMX_IBG
            _delay_us (DMX_IBG);
        #endif

		UDR0 = DMX_Master_0.getBuffer().getSlotValue( current_slot_0++ );
			
		// Send 512 channels
		if ( current_slot_0 >= DMX_MAX_FRAMESIZE ) {
            DMX_Master_0.setISRState(isr::DmxBreak);
	    }
        
		break;
    }
}

ISR(USART1_TX_vect)
{
    static uint16_t			current_slot_1;

	switch ( DMX_Master_1.getISRState() )
	{
	case isr::DmxBreak:
		UCSR1A = 0x0;
        UBRR1H = (unsigned char)(((F_CPU + DMX_BREAK_RATE * 8L) / (DMX_BREAK_RATE * 16L) - 1)>>8);
        UBRR1L = (unsigned char) ((F_CPU + DMX_BREAK_RATE * 8L) / (DMX_BREAK_RATE * 16L) - 1);
        UDR1   = 0x0;
        
        if ( DMX_Master_1.getISRState() ==  isr::DmxBreak ) {
            DMX_Master_1.setISRState(isr::DmxStartByte);
        }
        
        break;

	case isr::DmxStartByte:
		UCSR1A = 0x0;
        UBRR1H = (unsigned char)(((F_CPU + DMX_BAUD_RATE * 8L) / (DMX_BAUD_RATE * 16L) - 1)>>8);
		UBRR1L = (unsigned char) ((F_CPU + DMX_BAUD_RATE * 8L) / (DMX_BAUD_RATE * 16L) - 1);						
        current_slot_1 = 0;	
        UDR1 = DMX_Master_1.getBuffer()[ current_slot_1++ ];
        DMX_Master_1.setISRState(isr::DmxTransmitData);
		break;
	

	case isr::DmxTransmitData:
        // NOTE: we always send full frames of 513 bytes, this will bring us 
        // close to 40 frames / sec with no interslot delays
        #ifdef DMX_IBG
            _delay_us (DMX_IBG);
        #endif

		UDR1 = DMX_Master_1.getBuffer().getSlotValue( current_slot_1++ );
			
		// Send 512 channels
		if ( current_slot_1 >= DMX_MAX_FRAMESIZE ) {
            DMX_Master_1.setISRState(isr::DmxBreak);
	    }
        
		break;
    }
}

DMX_Master DMX_Master_0(0, 512, 2);
DMX_Master DMX_Master_1(1, 512, 2);
// DMX_Master DMX_Master_2(2);
// DMX_Master DMX_Master_3(3);
