#include "src/Artnet/Artnet.h"
#include "src/Artnet/Packets.h"
#include "src/Dmx/Dmx.h"

#define WEBDUINO_FAVICON_DATA ""

#include <WebServer.h>
#include <EEPROM.h>

#include "globals.h"

void softwareReset() {
  asm volatile ("  jmp 0");  
}

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //loadSettings();
  
  Artnet.begin(mac, ip, subnetMask, artnetNet, artnetSubnet, artnetUniverse, longName, shortName);

  Artnet.sendArtPollReply();

  DMX_Master_0.enable();
  DMX_Master_1.enable();
  DMX_Master_0.setChannelRange ( 1, 512, 127 );
  DMX_Master_1.setChannelRange ( 1, 512, 127 );

  startWebserver();
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}

void loop()  {
  uint8_t * data_0 = DMX_Master_0.getBuffer().getPointer();
  //uint8_t * data_1 = DMX_Master_1.getBuffer().getPointer();
  Artnet.read(data_0);
  webserver.processConnection();
}
