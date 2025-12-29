## IMPORTANT:
- [x] Feature: Wake-On-LAN support:
  - [x] write a "split" function and add it to the "utils.cpp" file
  - [x] convert the "mac_address" string obtained from the toml configs to a byte array before storing it in the "Host" object
  - [x] implement and use the "sendMagicPacket" function of "wifihandler"
- [x] Feature: Relay support (GND: GND, VCC: 3V3, OUT: D15)
- [x] Switch the toml configs with json configs
- [ ] Add "GetHostsJson": a command that can send the json configs to the web interface
- [ ] Improve the web interface and add it to the repository

## LATER:
- [ ] Add a README
- [ ] Add a documentation