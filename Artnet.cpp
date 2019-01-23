#include "Artnet.h"

Artnet::Artnet() = default;

void Artnet::begin(byte mac[], byte ip[], byte _broadcast[]) {
  Ethernet.begin(mac, ip);
  Udp.begin(ARTNET_PORT);
  broadcast = _broadcast;
}

void Artnet::begin(byte mac[], byte ip[], byte _broadcast[], char _longName[64]) {
  Artnet::begin(mac, ip, _broadcast);
  strcpy(longName, _longName);
}

void Artnet::begin(byte mac[], byte ip[], byte _broadcast[], char _longName[64], char _shortName[18]) {
  Artnet::begin(mac, ip, _broadcast, _longName);
  strcpy(shortName, _shortName);
}


uint16_t Artnet::read(uint8_t data[]) {

  uint16_t packetSize;
  packetSize = Udp.parsePacket();

  if (packetSize > MAX_ARTNET_BUFFER || packetSize <= 0) {
    return ERR_PACKET_DIMENSION;
  }

  Udp.read(artnetPacketHeader, ARTNET_DMX_START);

  // Check that packet ID is "Art-Net" else ignore
  for (byte i = 0 ; i < 9 ; i++) {
    if (artnetPacketHeader[i] != ARTNET_ID[i]) {
      return ERR_NOT_ARTNET;
    }
  }

  uint16_t opcode = artnetPacketHeader[8] | artnetPacketHeader[9] << 8;

  switch (opcode) {
    case OP_DMX: {
      uint8_t sequence = artnetPacketHeader[12];
      if (sequence < lastSequence && lastSequence - sequence < 0x80 ) {
        return ERR_OLD_PACKET;
      }

      uint16_t dmxDataLength = artnetPacketHeader[17] | artnetPacketHeader[16] << 8;

      Udp.read(data, dmxDataLength);
      lastSequence = sequence;
      break;
    }
    case OP_POLL: {
      artnetArtpollReply ArtPollReply;

      uint8_t id[8] = ARTNET_ID;
      memcpy(ArtPollReply.id, id, sizeof(ArtPollReply.id));

      ArtPollReply.opCode = OP_POLL_REPLY;

      IPAddress localIp = Ethernet.localIP();
      ArtPollReply.ip = static_cast<uint32_t>(localIp);

      ArtPollReply.port = ARTNET_PORT;

      ArtPollReply.versInfoH = 0;
      ArtPollReply.versInfoL = 1;

      ArtPollReply.netSwitch = 0;
      ArtPollReply.subSwitch = 0;

      ArtPollReply.oemHi = 0x00;
      ArtPollReply.oem = 0xff;

      ArtPollReply.ubeaVersion = 0;

      ArtPollReply.status1 = 0xe0;

      ArtPollReply.estaManLo = 0;
      ArtPollReply.estaManHi = 0;

      sprintf((char *)ArtPollReply.shortName, shortName);
      sprintf((char *)ArtPollReply.longName, longName);

      sprintf((char *)ArtPollReply.nodeReport, "1 Active Universe\0");

      ArtPollReply.numPortsHi = 0;
      ArtPollReply.numPortsLo = 1;

      memset(ArtPollReply.portTypes, 0x80, 1); //First port is implemented
      memset(ArtPollReply.portTypes+1, 0x00, 3); //Last 3 ports are not implemented

      memset(ArtPollReply.goodInput, 0x08, 4);
      memset(ArtPollReply.goodOutput, 0x80, 4);

      // TODO: make it configurable
      uint8_t swin[4] = {0x01,0x02,0x03,0x04};
      uint8_t swout[4] = {0x01,0x02,0x03,0x04};
      for(uint8_t i = 0; i < 4; i++)
      {
          ArtPollReply.swOut[i] = swout[i];
          ArtPollReply.swIn[i] = swin[i];
      }

      ArtPollReply.swVideo = 0;
      ArtPollReply.swMacro = 0;
      ArtPollReply.swRemote = 0;
      ArtPollReply.spare1 = 0;
      ArtPollReply.spare2 = 0;
      ArtPollReply.spare3 = 0;

      ArtPollReply.style = 0; // TODO

      // ArtPollReply.mac[6]; // TODO

      ArtPollReply.bindIp = static_cast<uint32_t>(localIp);

      ArtPollReply.bindIndex = 1;
      ArtPollReply.status2 = 0x0A; //wrong status

      memset(ArtPollReply.filler, 0, 26);

      Udp.beginPacket(broadcast, ARTNET_PORT); //send packet to broadcast address
      Udp.write((uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
      Udp.endPacket();
      break;
    }
    default:
      return ERR_NOT_EXISTING_OPCODE;
  }

  return opcode;
}