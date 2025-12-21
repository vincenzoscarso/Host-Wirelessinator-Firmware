#include <commandHandler.h>
#include <iostream>
#include <logHandler.h>
#include <map>
#include <string>

/**
 * Boot {host_name}
 * Reboot {host_name}
 * ForceShutdown {host_name}
 * GetStatus {host_name}
 * Informations
 * Help
 */

#define __STR_COMMAND_BOOT "Boot"
#define __STR_COMMAND_REBOOT "Reboot"
#define __STR_COMMAND_FORCESHUTDOWN "ForceShutdown"
#define __STR_COMMAND_GETSTATUS "GetStatus"
#define __STR_COMMAND_INFORMATIONS "Informations"
#define __STR_COMMAND_HELP "Help"

void __handleBootCommand(const std::string& host_name);
void __handleRebootCommand(const std::string& host_name);
void __handleForceShutdownCommand(const std::string& host_name);
void __handleGetStatusCommand(const std::string& host_name);
void __handleInformationsCommand();
void __handleHelpCommand();

std::map<std::string, void (*)(const std::string&)> commands_map = {
	{ __STR_COMMAND_BOOT, __handleBootCommand },
	{ __STR_COMMAND_REBOOT, __handleRebootCommand },
	{ __STR_COMMAND_FORCESHUTDOWN, __handleForceShutdownCommand },
	{ __STR_COMMAND_GETSTATUS, __handleGetStatusCommand }
};

bool commandHandler::checkForCommandAndExcecute(const std::string& command_name, const std::string& host_name) {
	auto command = commands_map.find(command_name);

	if (command != commands_map.end()) {
		command->second(host_name);
	} else if (command_name.compare(__STR_COMMAND_INFORMATIONS) == 0) {
		__handleInformationsCommand();
	} else if (command_name.compare(__STR_COMMAND_HELP) == 0) {
		__handleHelpCommand();
	} else {
		return false;
	}
	return true;
}

void __handleBootCommand(const std::string& host_name) {
	printInfoMessage("Booting system on host: %s", host_name.c_str());
}

void __handleRebootCommand(const std::string& host_name) {
	printInfoMessage("Rebooting system on host: %s", host_name.c_str());
}

void __handleForceShutdownCommand(const std::string& host_name) {
	printInfoMessage("Force shutting down system on host: %s", host_name.c_str());
}

void __handleGetStatusCommand(const std::string& host_name) {
	printInfoMessage("Getting status from host: %s", host_name.c_str());
}

void __handleInformationsCommand() {
	printInfoMessage("-- INFORMATIONS --");
}

void __handleHelpCommand() {
	printInfoMessage("-- HELP --");
}
