#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <string>
#include <ArduinoWebsockets.h>

namespace commandHandler {
bool checkForCommandAndExcecute(websockets::WebsocketsClient& client, const std::string& command_name, const std::string& host_name);
void handleNoCommand(websockets::WebsocketsClient& client, const std::string& message);
}

#endif /* COMMANDHANDLER_H */