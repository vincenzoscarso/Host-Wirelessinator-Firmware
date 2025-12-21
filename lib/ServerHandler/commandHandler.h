#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <string>

namespace commandHandler {
bool checkForCommandAndExcecute(const std::string& command_name, const std::string& host_name);
}

#endif /* COMMANDHANDLER_H */