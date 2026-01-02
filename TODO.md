## IMPORTANT:
- [x] Feature: Wake-On-LAN support:
  - [x] write a "split" function and add it to the "utils.cpp" file
  - [x] convert the "mac_address" string obtained from the toml configs to a byte array before storing it in the "Host" object
  - [x] implement and use the "sendMagicPacket" function of "wifihandler"
- [x] Feature: Relay support (GND: GND, VCC: 3V3, OUT: D15)
- [x] Switch the toml configs with json configs
- [x] Add "GetHostsJson": a command that can send the json configs to the web interface
- [x] Improve the web interface and add it to the repository
- [ ] Web-UI:
  - [ ] Show arduino status
  - [ ] Create class for global values
  - [ ] Divide the js in multiple files (if possible)
  - [ ] Choose better colors
- [ ] Fix: when client send various commands too quickly server does nothing, everything freezes 

## LATER:
- [ ] Add a README
- [ ] Add a documentation