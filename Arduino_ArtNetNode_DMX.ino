/*
 * EEPROM
 * ip: 4 byte
 * subnet-mask: 4 byte
 * artnet-Net: 1 byte
 * artnet-SubNet: 1 byte
 * Artnet-Universe: 1 byte
 * node name: 18 byte
 * node long name: 64 byte
 */

#include "Artnet.h"
#include "Packets.h"

#include <DMXSerial.h>

#include "globals.h"

void setup()
{
  Artnet.begin(mac, ip, subnetMask, artnetNet, artnetSubnet, artnetUniverse, longName, shortName);
  DMXSerial.init(DMXController);

  Artnet.sendArtPollReply();
  //TODO: Start web server
}

void loop()
{
  uint8_t * data = DMXSerial.getBuffer() + 1;
  Artnet.read(data);
}
