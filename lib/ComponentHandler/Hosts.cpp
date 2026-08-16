#include "IPAddress.h"
#include <Hosts.h>
#include <logHandler.h>
#include <nlohmann/json.hpp>
#include <secrets.h>
#include <stdexcept>
#include <string>
using json = nlohmann::json;

std::vector<Host> Hosts::hosts;

std::basic_string<char> __getNameValue(std::string key);
std::basic_string<char> __getTypeValue(std::string key);
bool __getUseRelayValue(std::string key);
int __getRelayPinValue(std::string key);
bool __getUseMagicPacketValue(std::string key);
IPAddress __getIpAddressValue(std::string key);
std::basic_string<char> __getMacAddressValue(std::string key);

bool Hosts::updateHostsVector() {
	if (Hosts::getNumberOfHosts() <= 0) {
		printErrorMessage("Incorrect number of hosts: less or equal to 0, minimum is 1");
		return false;
	}

	for (int i = 0; i < Hosts::getNumberOfHosts(); i++) {
		std::string key = std::to_string(i);

		try {
			hosts.emplace_back(
			    __getNameValue(key),
			    __getTypeValue(key),
			    __getUseRelayValue(key),
			    __getRelayPinValue(key),
			    __getUseMagicPacketValue(key),
			    __getIpAddressValue(key),
			    __getMacAddressValue(key));
		} catch (const std::exception& e) {
			printErrorMessage("Failed to obtain values from the table: %s", e.what());
			return false;
		}
	}

	return true;
}

int Hosts::getNumberOfHosts() {
	return secrets::hosts_json.value("number_of_hosts", 0);
}

bool Hosts::isHostNameValid(const std::string& host_name) {
	for (int i = 0; i < Hosts::getNumberOfHosts(); i++) {
		if (host_name == Hosts::hosts[i].getName()) {
			return true;
		}
	}
	return false;
}

int Hosts::getHostVectorIndexFromHostName(const std::string& host_name) {
	for (int i = 0; i < Hosts::getNumberOfHosts(); i++) {
		if (host_name == Hosts::hosts[i].getName()) {
			return i;
		}
	}
	throw std::invalid_argument("Couldn't find the host object: inexistent host name");
}

// GETTERS ################################

std::basic_string<char> __getNameValue(std::string key) {
	return secrets::hosts_json[key].value("name", "");
}

std::basic_string<char> __getTypeValue(std::string key) {
	return secrets::hosts_json[key].value("type", "");
}

bool __getUseRelayValue(std::string key) {
	return secrets::hosts_json[key]["control_options"].value("use_relay", false);
}

int __getRelayPinValue(std::string key) {
	return secrets::hosts_json[key]["control_options"].value("relay_pin", -1);
}

bool __getUseMagicPacketValue(std::string key) {
	return secrets::hosts_json[key]["control_options"].value("use_magic_packet", false);
}

IPAddress __getIpAddressValue(std::string key) {
	IPAddress ip_address;
	ip_address.fromString(secrets::hosts_json[key]["addresses"].value("ip_address", "").c_str());
	return ip_address;
}

std::basic_string<char> __getMacAddressValue(std::string key) {
	return secrets::hosts_json[key]["addresses"].value("mac_address", "");
}
