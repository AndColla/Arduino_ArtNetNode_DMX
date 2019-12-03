#ifndef _DMX_
#define _DMX_

#include <Arduino.h>
#include <inttypes.h>

#define DMX_MAX_FRAME_SIZE      513     // Startbyte + 512 Slots
#define DMX_MIN_FRAME_SIZE      2       // Startbyte + 1 Slot

#define DMX_MAX_FRAME_CHANNELS  512     // Maxmim number of channer per frame

#define DMX_STARTCODE_SIZE      1       // Size of startcode in bytes

#define DMX_START_CODE          0x0     // Start code for a DMX frame

#define DMX_MAX_FRAMESIZE       513     // Startbyte + 512 Slots
#define DMX_MIN_FRAMESIZE       2       // Startbyte + 1 Slot

#define DMX_BREAK_RATE          49950
#define DMX_BAUD_RATE 		    250000

namespace isr
{
    enum isrState
    {
        DmxBreak,
        DmxStartByte,
        DmxTransmitData
    };

    enum isrMode
    {
        Disabled,
        DMXTransmit
    };
};

class DMX_Buffer
{
    public:
        DMX_Buffer     ( uint16_t buffer_size );
        DMX_Buffer     ( DMX_Buffer &buffer );
        ~DMX_Buffer    ( void );

        uint16_t getBufferSize ( void );

        uint8_t getSlotValue ( uint16_t index );
        void    setSlotValue ( uint16_t index, uint8_t value );
        void    setSlotRange ( uint16_t start, uint16_t end, uint8_t value );
        void    clear ( void ); 
        uint8_t *getPointer ( void );

        uint8_t &operator[] ( uint16_t index );

    private:

        uint8_t     *m_refcount;
        uint16_t    m_bufferSize;
        uint8_t     *m_buffer;      
};

class DMX_Master
{
    public:
        DMX_Master (int uart, uint16_t maxChannel, int controlPin);
    
        void enable  ( void );
        void disable ( void );

        // Get reference to the internal framebuffer
        DMX_Buffer &getBuffer ( void );

        // Update channel values
        void setChannelValue ( uint16_t channel, uint8_t value );
        void setChannelRange ( uint16_t start, uint16_t end, uint8_t value );

        void setISRMode ( isr::isrMode );

        isr::isrState getISRState ( void );
        void setISRState ( isr::isrState );

    protected:
        void setStartCode ( uint8_t value ); 

    private:
        DMX_Buffer      m_frameBuffer;
        uint8_t         m_controlPin;
        uint8_t         m_uart;
        isr::isrState   m_isrState;
};

extern DMX_Master DMX_Master_0;
extern DMX_Master DMX_Master_1;
// extern DMX_Master DMX_Master_2;
// extern DMX_Master DMX_Master_3;

#endif