#include "IPAddress.h"
#include <Hosts.h>
#include <logHandler.h>
#include <nlohmann/json.hpp>
#include <secrets.h>
#include <stdexcept>
#include <string>
using json = nlohmann::json;

std::vector<Host> Hosts::hosts;
IPAddress __getHostIp(std::string key);

bool Hosts::updateHostsVector() {
	if (Hosts::getNumberOfHosts() <= 0) {
		printErrorMessage("Incorrect number of hosts: less or equal to 0, minimum is 1");
		return false;
	}

	for (int i = 0; i < Hosts::getNumberOfHosts(); i++) {
		std::string key = std::to_string(i);

		try {
			hosts.emplace_back(
			    secrets::hosts_json[key].value("name", ""),
			    secrets::hosts_json[key].value("type", ""),
			    secrets::hosts_json[key]["control_options"].value("use_relay", false),
			    secrets::hosts_json[key]["control_options"].value("relay_pin", -1),
			    secrets::hosts_json[key]["control_options"].value("use_magic_packet", false),
			    __getHostIp(key),
			    secrets::hosts_json[key]["addresses"].value("mac_address", ""));
		} catch (const std::exception& e) {
			printErrorMessage("Failed to obtain values from the table: %s", e.what());
			return false;
		}
	}

	return true;
}

/**
 * @returns number of hosts if existent, else 0
 */
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

IPAddress __getHostIp(std::string key) {
	IPAddress ip_address;
	ip_address.fromString(secrets::hosts_json[key]["addresses"].value("ip_address", "").c_str());
	return ip_address;
}
