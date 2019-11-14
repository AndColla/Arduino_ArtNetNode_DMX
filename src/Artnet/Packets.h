#ifndef ARTNET_PACKETS_H
#define ARTNET_PACKETS_H

#define ARTNET_MAX_PORTS 4
#define ARTNET_DMX_LENGTH 512

typedef struct {
  uint8_t  id[8];
  uint16_t opCode;
  uint32_t ip;
  uint16_t port;
  uint8_t  versInfoH;
  uint8_t  versInfoL;
  uint8_t  netSwitch;
  uint8_t  subSwitch;
  uint8_t  oemHi;
  uint8_t  oem;
  uint8_t  ubeaVersion;
  uint8_t  status1;
  uint8_t  estaManLo;
  uint8_t  estaManHi;
  uint8_t  shortName[18];
  uint8_t  longName[64];
  uint8_t  nodeReport[64];
  uint8_t  numPortsHi; //max of 4 ports per node
  uint8_t  numPortsLo; //max of 4 ports per node
  uint8_t  portTypes[ARTNET_MAX_PORTS]; //max of 4 ports per node
  uint8_t  goodInput[4];
  uint8_t  goodOutput[4];
  uint8_t  swIn[4];
  uint8_t  swOut[4];
  uint8_t  swVideo;
  uint8_t  swMacro;
  uint8_t  swRemote;
  uint8_t  spare1;
  uint8_t  spare2;
  uint8_t  spare3;
  uint8_t  style;
  uint8_t  mac[6];
  uint32_t bindIp;
  uint8_t  bindIndex;
  uint8_t  status2;
  uint8_t  filler[26];
}__attribute__((packed)) artnetArtpollReply;

typedef struct {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  protVerHi;
  uint8_t  protVerLo;
  uint8_t  sequence;
  uint8_t  physical;
  uint8_t  subUni;
  uint8_t  net;
  uint8_t  lengthHi;
  uint8_t  length;
  uint8_t  data[ARTNET_DMX_LENGTH];
} __attribute__((packed)) artnetDmx;

#endif