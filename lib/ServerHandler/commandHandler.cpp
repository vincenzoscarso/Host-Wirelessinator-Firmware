#include <Host.h>
#include <Hosts.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <iostream>
#include <logHandler.h>
#include <map>
#include <secrets.h>
#include <string>
#include <wifiHandler.h>

#define __STR_COMMAND_BOOT "Boot"
#define __STR_COMMAND_REBOOT "Reboot"
#define __STR_COMMAND_FORCESHUTDOWN "ForceShutdown"
#define __STR_COMMAND_GETSTATUS "GetStatus"
#define __STR_COMMAND_GETHOSTSJSON "GetHostsJson"
#define __STR_COMMAND_INFORMATIONS "Informations"
#define __STR_COMMAND_HELP "Help"

bool __checkForCommandAndExcecuteInHostCommandsMap(websockets::WebsocketsClient& client, const std::string& command_name, const std::string& host_name);
bool __checkForCommandAndExcecuteInDeviceCommandsMap(websockets::WebsocketsClient& client, const std::string& command_name);

void __handleBootCommand(websockets::WebsocketsClient& client, Host host);
void __handleRebootCommand(websockets::WebsocketsClient& client, Host host);
void __handleForceShutdownCommand(websockets::WebsocketsClient& client, Host host);
void __handleGetStatusCommand(websockets::WebsocketsClient& client, Host host);
void __handleGetHostsJsonCommand(websockets::WebsocketsClient& client);
void __handleInformationsCommand(websockets::WebsocketsClient& client);
void __handleHelpCommand(websockets::WebsocketsClient& client);

/*========
    Maps
  ========*/

std::map<std::string, void (*)(websockets::WebsocketsClient&, Host)> host_commands_map = {
	{ __STR_COMMAND_BOOT, __handleBootCommand },
	{ __STR_COMMAND_REBOOT, __handleRebootCommand },
	{ __STR_COMMAND_FORCESHUTDOWN, __handleForceShutdownCommand },
	{ __STR_COMMAND_GETSTATUS, __handleGetStatusCommand }
};

std::map<std::string, void (*)(websockets::WebsocketsClient&)> device_commands_map = {
	{ __STR_COMMAND_GETHOSTSJSON, __handleGetHostsJsonCommand },
	{ __STR_COMMAND_INFORMATIONS, __handleInformationsCommand },
	{ __STR_COMMAND_HELP, __handleHelpCommand }
};

/*===========================
    Maps handling functions
  ===========================*/

bool commandHandler::checkForCommandAndExcecute(websockets::WebsocketsClient& client, const std::string& command_name, const std::string& host_name) {
	if (__checkForCommandAndExcecuteInHostCommandsMap(client, command_name, host_name)) {
		return true;
	} else if (__checkForCommandAndExcecuteInDeviceCommandsMap(client, command_name)) {
		return true;
	}
	return false;
}

bool __checkForCommandAndExcecuteInHostCommandsMap(websockets::WebsocketsClient& client, const std::string& command_name, const std::string& host_name) {
	auto command = host_commands_map.find(command_name);

	if (command != host_commands_map.end()) {
		if (!Hosts::isHostNameValid(host_name)) {
			printErrorMessage(true, "Invalid host name '%s'", host_name.c_str());
			return false;
		}

		command->second(client, Hosts::hosts[Hosts::getHostVectorIndexFromHostName(host_name)]);
		return true;
	}
	return false;
}

bool __checkForCommandAndExcecuteInDeviceCommandsMap(websockets::WebsocketsClient& client, const std::string& command_name) {
	auto command = device_commands_map.find(command_name);
	if (command != device_commands_map.end()) {
		command->second(client);
		return true;
	}

	return false;
}

/*============
    Handlers
  ============*/

void commandHandler::handleNoCommand(websockets::WebsocketsClient& client, const std::string& message) {
	printErrorMessage(true, "Unknown command: %s", message.c_str());
}

/*----------------------------
    Hosts' commands handlers
  ----------------------------*/

void __handleBootCommand(websockets::WebsocketsClient& client, Host host) {
	printInfoMessage(true,"Booting system on host: %s", host.getName().c_str());

	if (host.isUseMagicPacketEnabled()) {
		WifiHandler::sendMagicPacket(host.getMacAddress());
	} else if (host.isUseRelayPinEnabled()) {
		componentHandler::setHostRelayPinStatus(host, LOW);
		delay(1000);
		componentHandler::setHostRelayPinStatus(host, HIGH);
	} else {
		printErrorMessage(true, "Couldn't find a method to boot this host, check configuration");
	}
}

void __handleRebootCommand(websockets::WebsocketsClient& client, Host host) {
	printErrorMessage(true, "Reboot command is currently not implemented");
}

void __handleForceShutdownCommand(websockets::WebsocketsClient& client, Host host) {
	printErrorMessage(true, "ForceShutdown command is currently not implemented");

	/* printInfoMessage("Force shutting down system on host: %s", host.getName().c_str());

	if (host.isUseRelayPinEnabled()) {
		componentHandler::setHostRelayPinStatus(host, HIGH);
		delay(5000);
		componentHandler::setHostRelayPinStatus(host, LOW);
	} else {
		printErrorMessage("Couldn't force-shutdown this host: relay needed, check configuration");
	} */
}

void __handleGetStatusCommand(websockets::WebsocketsClient& client, Host host) {
	printErrorMessage(true, "GetStatus command is currently not implemented");
}

/*------------------------------
    Decice's commands handlers
  ------------------------------*/

void __handleGetHostsJsonCommand(websockets::WebsocketsClient& client) {
	printInfoMessage("Sending hosts JSON to the client:\n%s", secrets::hosts_json.dump().c_str());
	client.send(secrets::hosts_json.dump().c_str());
}

void __handleInformationsCommand(websockets::WebsocketsClient& client) {
	printErrorMessage(true, "Informations command is currently not implemented");
}

void __handleHelpCommand(websockets::WebsocketsClient& client) {
	printErrorMessage(true, "Help command is currently not implemented");
}
