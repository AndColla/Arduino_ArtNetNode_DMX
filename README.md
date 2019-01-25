# Arduino ArtNetNode DMX
The actual code works on Arduino Uno with an Ethernet Shield.
## Wiring
```
arduino       MAX485
pin 1 (tx)    pin 4(DI)
pin 2         pin 2 + pin 3 (RE + DE)
```
## TODO
- Use the EEPROM memory to store the settigs of the node
- Create a web interface to change the settings
- Check Net, Subnet and Universe of the artnet packet
- Clean up the code
