### Todo
For the Arduino websockets docs check https://github.com/gilmaimon/TinyWebsockets/wiki 

- [ ] implement informations and help commands
- [ ] implement heartbeat to detect client disconnection instead of using delays
- [ ] implement an handler for client.onEvent() function to boost performance
- [ ] Add a command to reboot arduino

### Todo later
- [ ] Use HTTP instead of websockets
- [ ] make command handling non-blocking (HTTP should solve this)
- [ ] when logging is disabled messages that need to go to the client MUST be sent
- [ ] Add a documentation

### Done
- [x] Divide the css in multiple files
- [x] Divide the js in multiple files
- [x] Show arduino status
- [x] Fix: host width for mobile
- [x] Improve typescript code modularity
- [x] Feature: Wake-On-LAN support:
  - [x] write a "split" function and add it to the "utils.cpp" file
  - [x] convert the "mac_address" string obtained from the toml configs to a byte array before storing it in the "Host" object
  - [x] implement and use the "sendMagicPacket" function of "wifihandler"
- [x] Feature: Relay support (GND: GND, VCC: 3V3, OUT: D15)
- [x] Switch the toml configs with json configs
- [x] Add "GetHostsJson": a command that can send the json configs to the web interface
- [x] Improve the web interface and add it to the repository
- [x] Fix: server loop freezing
- [x] Make code non-blocking:
  - [x] Fix: __acceptNewConnections() should not be blocking
  - [x] Fix: __rejectNewConnections() should not be blocking
- [x] Handle wifi disconnection
- [x] Implement "Modem-sleep" to make the Arduino use less energy when idling
- [x] Improve logging:
  - [x] Implement option to send logs to client
  - [x] Add a "printDebugInfo" function
- [x] Add a README
