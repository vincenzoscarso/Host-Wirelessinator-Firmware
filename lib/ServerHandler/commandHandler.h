#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <string>
#include <ArduinoWebsockets.h>

namespace commandHandler {
bool checkForCommandAndExcecute(websockets::WebsocketsClient& client, const std::string& command_name, const std::string& host_name);
}

#endif /* COMMANDHANDLER_H */