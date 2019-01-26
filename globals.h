byte ip[4] = {10, 0, 1, 199};
byte subnetMask[4] = {255, 255, 255, 0};
byte mac[6] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC}; // generate new mac with local flag

byte artnetNet = 0;
byte artnetSubnet = 0;
byte artnetUniverse = 1;

char shortName[18] = "Arduino Art-Net";
char longName[64] = "Arduino Art-Net Node";

Artnet Artnet;
DMX_Master DMXMaster(512, 2);
WebServer webserver("", 80);
