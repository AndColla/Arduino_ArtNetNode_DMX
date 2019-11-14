#include "src/Artnet/Artnet.h"
#include "src/Artnet/Packets.h"
#include "src/Conceptinetics/Conceptinetics.h"

#define WEBDUINO_FAVICON_DATA ""

#include <WebServer.h>
#include <EEPROM.h>

#include "globals.h"

void softwareReset() {
  asm volatile ("  jmp 0");  
}

void setup() {
  // disable the SD card by switching pin 4 high
  // not using the SD card in this program, but if an SD card is left in the socket,
  // it may cause a problem with accessing the Ethernet chip, unless disabled
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  loadSettings();
  
  Artnet.begin(mac, ip, subnetMask, artnetNet, artnetSubnet, artnetUniverse, longName, shortName);

  Artnet.sendArtPollReply();

  DMXMaster.enable();

  startWebserver();
}

void loop()  {
  uint8_t * data = DMXMaster.getBuffer().getPointer();
  Artnet.read(data);
  webserver.processConnection();
}
