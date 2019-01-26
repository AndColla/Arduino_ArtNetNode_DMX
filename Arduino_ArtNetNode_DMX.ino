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

#include <WebServer.h>
#include <Conceptinetics.h>

#include "globals.h"

void setup()
{
  // disable the SD card by switching pin 4 high
  // not using the SD card in this program, but if an SD card is left in the socket,
  // it may cause a problem with accessing the Ethernet chip, unless disabled
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  Artnet.begin(mac, ip, subnetMask, artnetNet, artnetSubnet, artnetUniverse, longName, shortName);

  DMXMaster.enable();

  Artnet.sendArtPollReply();
  startWebserver();
}

void loop()
{
  uint8_t * data = DMXMaster.getBuffer().getPointer();
  Artnet.read(data);
  webserver.processConnection();
}
