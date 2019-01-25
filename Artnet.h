#ifndef ARTNET_H
#define ARTNET_H

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <string.h>

#include "Packets.h"

#define ARTNET_PORT 6454
// OP Codes
#define OP_POLL 0x2000
#define OP_POLL_REPLY 0x2100
#define OP_DMX 0x5000
// Error Codes
#define ERR_PACKET_DIMENSION 0x1000
#define ERR_NOT_ARTNET 0x1001
#define ERR_NOT_EXISTING_OPCODE 0x1002
#define ERR_OLD_PACKET 0x1003

#define MAX_ARTNET_BUFFER 530 // 18 (Art-Net Headers) + 512 (DMX Channels)
#define ARTNET_ID "Art-Net\0"
#define ARTNET_DMX_START 18 // Start point for dmx data in the Art-Net packet
#define ARTNET_HEADER 17 // End point for Art-Net header

class Artnet {
  public:
    Artnet();

    void begin(byte mac[], byte ip[], byte _broadcast[]);
    void begin(byte mac[], byte ip[], byte _broadcast[], char _longName[64]);
    void begin(byte mac[], byte ip[], byte _broadcast[], char _longName[64], char _shortName[18]);
    uint16_t read(uint8_t data[]);
    void sendArtPollReply();

  private:
    EthernetUDP Udp;

    char shortName[18] = "Arduino Art-Net\0";
    char longName[64] = "Arduino Art-Net Node\0";

    IPAddress broadcast;

    uint8_t artnetPacketHeader[ARTNET_DMX_START];
    uint8_t lastSequence;
};

#endif
