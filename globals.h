byte ip[] = {10, 0, 1, 199};
byte subnetMask[] = {255, 255, 255, 0};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC}; // generate new mac with local flag

byte artnetNet = 0;
byte artnetSubnet = 0;
byte artnetUniverse = 0;

char shortName[18] = "Arduino Art-Net";
char longName[64] = "Arduino Art-Net Node";

Artnet Artnet;
