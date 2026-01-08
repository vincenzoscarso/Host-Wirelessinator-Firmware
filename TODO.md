## IMPORTANT

### Web-UI:
- [x] Divide the css in multiple files
- [x] Divide the js in multiple files
- [x] Show arduino status
- [ ] Fix: host width for mobile

### Arduino:
- [x] Feature: Wake-On-LAN support:
  - [x] write a "split" function and add it to the "utils.cpp" file
  - [x] convert the "mac_address" string obtained from the toml configs to a byte array before storing it in the "Host" object
  - [x] implement and use the "sendMagicPacket" function of "wifihandler"
- [x] Feature: Relay support (GND: GND, VCC: 3V3, OUT: D15)
- [x] Switch the toml configs with json configs
- [x] Add "GetHostsJson": a command that can send the json configs to the web interface
- [x] Improve the web interface and add it to the repository
- [ ] Fix: when client send various commands too quickly server does nothing, everything freezes
- [ ] Add a command to reboot arduino

## LATER:
- [ ] Choose better colors
- [ ] Add a README
- [ ] Add a documentation
- [ ] Improve logging:
  - [ ] Accept std::string as a parameter
  - [ ] Add a "printDebugInfo" function
