#include <Host.h>
#include <Hosts.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <iostream>
#include <logHandler.h>
#include <map>
#include <string>
#include <wifiHandler.h>

#define __STR_COMMAND_BOOT "Boot"
#define __STR_COMMAND_REBOOT "Reboot"
#define __STR_COMMAND_FORCESHUTDOWN "ForceShutdown"
#define __STR_COMMAND_GETSTATUS "GetStatus"
#define __STR_COMMAND_INFORMATIONS "Informations"
#define __STR_COMMAND_HELP "Help"

void __handleBootCommand(websockets::WebsocketsClient client, Host host);
void __handleRebootCommand(websockets::WebsocketsClient client, Host host);
void __handleForceShutdownCommand(websockets::WebsocketsClient client, Host host);
void __handleGetStatusCommand(websockets::WebsocketsClient client, Host host);
void __handleInformationsCommand(websockets::WebsocketsClient client);
void __handleHelpCommand(websockets::WebsocketsClient client);

std::map<std::string, void (*)(websockets::WebsocketsClient, Host)> commands_map = {
	{ __STR_COMMAND_BOOT, __handleBootCommand },
	{ __STR_COMMAND_REBOOT, __handleRebootCommand },
	{ __STR_COMMAND_FORCESHUTDOWN, __handleForceShutdownCommand },
	{ __STR_COMMAND_GETSTATUS, __handleGetStatusCommand }
};

bool commandHandler::checkForCommandAndExcecute(websockets::WebsocketsClient client, const std::string& command_name, const std::string& host_name) {
	auto command = commands_map.find(command_name);

	if (command != commands_map.end()) {
		if (!Hosts::isHostNameValid(host_name)) {
			printErrorMessage("Invalid host name '%s'", host_name.c_str());
			return false;
		}

		command->second(client, Hosts::hosts[Hosts::getHostVectorIndexFromHostName(host_name)]);
	} else if (command_name.compare(__STR_COMMAND_INFORMATIONS) == 0) {
		__handleInformationsCommand(client);
	} else if (command_name.compare(__STR_COMMAND_HELP) == 0) {
		__handleHelpCommand(client);
	} else {
		return false;
	}
	return true;
}

/*============
    Handlers
  ============*/

void __handleBootCommand(websockets::WebsocketsClient client, Host host) {
	printInfoMessage("Booting system on host: %s", host.getName().c_str());

	if (host.isUseMagicPacketEnabled()) {
		WifiHandler::sendMagicPacket(host.getMacAddress());
	} else if (host.isUseRelayPinEnabled()) {
		componentHandler::setHostRelayPinStatus(host, HIGH);
		delay(1000);
		componentHandler::setHostRelayPinStatus(host, LOW);
	} else {
		printErrorMessage("Couldn't find a method to boot this host, check configuration");
	}
}

void __handleRebootCommand(websockets::WebsocketsClient client, Host host) {
	printErrorMessage("Reboot command is currently not implemented");
}

void __handleForceShutdownCommand(websockets::WebsocketsClient client, Host host) {
	printInfoMessage("Force shutting down system on host: %s", host.getName().c_str());

	if (host.isUseRelayPinEnabled()) {
		componentHandler::setHostRelayPinStatus(host, HIGH);
		delay(5000);
		componentHandler::setHostRelayPinStatus(host, LOW);
	} else {
		printErrorMessage("Couldn't force-shutdown this host: relay needed, check configuration");
	}
}

void __handleGetStatusCommand(websockets::WebsocketsClient client, Host host) {
	printErrorMessage("GetStatus command is currently not implemented");
	
}

void __handleInformationsCommand(websockets::WebsocketsClient client) {
	printErrorMessage("Informations command is currently not implemented");
}

void __handleHelpCommand(websockets::WebsocketsClient client) {
	printErrorMessage("Help command is currently not implemented");
}
