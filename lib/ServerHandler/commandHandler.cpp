#include "IPAddress.h"
#include "WiFiClient.h"
#include "utils.h"
#include <ESP32Ping.h>
#include <Host.h>
#include <Hosts.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <iostream>
#include <logHandler.h>
#include <map>
#include <nlohmann/json.hpp>
#include <secrets.h>
#include <string>
#include <wifiHandler.h>

using nlohmann::json;

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

std::string __getResponse(std::string header, std::string body);

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
	std::string body;

	if (host.isUseMagicPacketEnabled()) {
		body = "Booting system with magic packet on host: ";
		WifiHandler::sendMagicPacket(host.getMacAddress());
	} else if (host.isUseRelayPinEnabled()) {
		body = "Booting system with relay on host: ";
		componentHandler::setHostRelayPinStatus(host, LOW);
		delay(1000);
		componentHandler::setHostRelayPinStatus(host, HIGH);
	} else {
		printErrorMessage(true, "Couldn't find a method to boot this host, check configuration");
		return;
	}

	body += host.getName().c_str();
	std::string response = __getResponse("Boot", body);
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

void __handleRebootCommand(websockets::WebsocketsClient& client, Host host) {
	std::string body = "Reboot command is currently not implemented for host: " + host.getName();
	std::string response = __getResponse("Reboot", body);
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

void __handleForceShutdownCommand(websockets::WebsocketsClient& client, Host host) {
	std::string body = "ForceShutdown command is currently not implemented for host: " + host.getName();
	std::string response = __getResponse("ForceShutdown", body);
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());

	/* printInfoMessage(true, "Force shutting down system on host: %s", host.getName().c_str());

	if (host.isUseRelayPinEnabled()) {
	    componentHandler::setHostRelayPinStatus(host, HIGH);
	    delay(5000);
	    componentHandler::setHostRelayPinStatus(host, LOW);
	} else {
	    printErrorMessage(true, "Couldn't force-shutdown this host: relay needed, check configuration");
	} */
}

void __handleGetStatusCommand(websockets::WebsocketsClient& client, Host host) {
	json body;
	body["Host"] = host.getName();
	body["Status"] = Ping.ping(host.getIpAddress()) ? "Online" : "Offline";

	std::string response = __getResponse("GetStatus", body.dump());
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

/*------------------------------
    Decice's commands handlers
  ------------------------------*/

void __handleGetHostsJsonCommand(websockets::WebsocketsClient& client) {
	std::string response = __getResponse("GetHostsJson", secrets::hosts_json.dump());
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

void __handleInformationsCommand(websockets::WebsocketsClient& client) {
	json body;
	body["Version"] = utils::toStdString(HOST_WIRELESSINATOR_FIRMWARE_VERSION);
	body["Board"] = utils::toStdString(ARDUINO_BOARD);
	body["Developer"] = "Vincenzo Scarso";
	body["Link"] = "github.com/vincenzoscarso";

	auto response = __getResponse("Informations", body.dump());
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

void __handleHelpCommand(websockets::WebsocketsClient& client) {
	json body;
	body["commands"] = {
		"Boot {host_name}",
		"Reboot {host_name}",
		"ForceShutdown {host_name}",
		"GetStatus {host_name}",
		"GetHostsJson",
		"Informations",
		"Help"
	};

	auto response = __getResponse("Help", body.dump());
	client.send(response.c_str());
	printInfoMessage(true, "Sent response: %s", response.c_str());
}

/*===========
    Helpers
  ===========*/

/**
Omit last newline of the header
*/
std::string __getResponse(std::string header, std::string body) {
	std::string response = header + "\n-- HEADER END --\n" + body;
	return response.c_str();
}