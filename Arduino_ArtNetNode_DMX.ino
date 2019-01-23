/*
 * EEPROM
 * ip: 4 byte
 * subnet-mask: 4 byte
 * artnet-Net: 1 byte
 * artnet-SubNet: 1 byte
 * Artnet-Universe: 1 byte
 * artnet/sACN (?): 1 bit
 * node name: 18 byte
 * node long name: 64 byte
 */

#include "Artnet.h"
#include "Packets.h"

#include <DMXSerial.h>

byte ip[] = {10, 0, 1, 199};
byte broadcast[] = {10, 0, 1, 255};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC}; // generate new mac with local flag

Artnet artnet;

void setup()
{
  artnet.begin(mac, ip, broadcast);
  DMXSerial.init(DMXController);

  //TODO: Send artpoll reply
  //TODO: Start web server
}

void loop()
{
  uint8_t * data = DMXSerial.getBuffer() + 1;
  artnet.read(data);
}
