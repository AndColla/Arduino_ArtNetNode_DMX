bool loadSettings() {  
  // Check if we have previous saves.  If not, return false
  if(EEPROM.read(200) != 'O' || EEPROM.read(201) != 'K') {
    saveIpSettings();
    saveArtnetSettings();
    return false;
  }
  
  ip[0] = EEPROM.read(10);
  ip[1] = EEPROM.read(11);
  ip[2] = EEPROM.read(12);
  ip[3] = EEPROM.read(13);
  subnetMask[0] = EEPROM.read(20);
  subnetMask[1] = EEPROM.read(21);
  subnetMask[2] = EEPROM.read(22);
  subnetMask[3] = EEPROM.read(23);

  artnetNet = EEPROM.read(30);
  artnetSubnet = EEPROM.read(31);
  artnetUniverse = EEPROM.read(32);

  // 40 to 57 -> node name
  // 60 to 123 -> node long name

  return true;
}

bool saveIpSettings() {
  EEPROM.write(200, '\0');
  EEPROM.write(201, '\0');
  
  if(EEPROM.read(200) != '\0'|| EEPROM.read(201) != '\0') {
    return false;
  }
  
  EEPROM.update(10, ip[0]);
  EEPROM.update(11, ip[1]);
  EEPROM.update(12, ip[2]);
  EEPROM.update(13, ip[3]);
  
  EEPROM.update(20, subnetMask[0]);
  EEPROM.update(21, subnetMask[1]);
  EEPROM.update(22, subnetMask[2]);
  EEPROM.update(23, subnetMask[3]);

  EEPROM.write(200, 'O');
  EEPROM.write(201, 'K');

  // Verify our OK was written & return false if not
  if(EEPROM.read(200) != 'O' || EEPROM.read(201) != 'K') {
    return false;
  }

  return true;
}

bool saveArtnetSettings() {
  EEPROM.write(200, '\0');
  EEPROM.write(201, '\0');
  
  if(EEPROM.read(200) != '\0'|| EEPROM.read(201) != '\0') {
    return false;
  }
  
  EEPROM.update(30, artnetNet);
  EEPROM.update(31, artnetSubnet);
  EEPROM.update(32, artnetUniverse);

  EEPROM.write(200, 'O');
  EEPROM.write(201, 'K');

  // Verify our OK was written & return false if not
  if(EEPROM.read(200) != 'O' || EEPROM.read(201) != 'K') {
    return false;
  }

  return true;
}
