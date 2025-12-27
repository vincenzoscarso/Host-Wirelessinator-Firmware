#include <Hosts.h>
#include <logHandler.h>
#include <secrets.h>
#include <toml++/toml.hpp>
#include <stdexcept>

std::vector<Host> Hosts::hosts;

static toml::parse_result& __get_hosts_table();

bool Hosts::updateHostsVector() {
	if (Hosts::getNumberOfHosts() <= 0) {
		printErrorMessage("Incorrect number of hosts: less or equal to 0, minimum is 1");
		return false;
	}

	for (int i = 0; i < Hosts::getNumberOfHosts(); i++) {
		std::string key = std::to_string(i);

		try {
			hosts.emplace_back(
			    __get_hosts_table()[key]["name"].value_or<std::string>(""),
			    __get_hosts_table()[key]["type"].value_or<std::string>(""),
			    __get_hosts_table()[key]["control_options"]["use_relay"].value_or<bool>(false),
			    __get_hosts_table()[key]["control_options"]["relay_pin"].value_or<int>(-1),
			    __get_hosts_table()[key]["control_options"]["use_magic_packet"].value_or<bool>(false),
			    __get_hosts_table()[key]["control_options"]["mac_address"].value_or<std::string>(""));
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
	return __get_hosts_table()["number_of_hosts"].value_or<int>(0);
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

static toml::parse_result& __get_hosts_table() {
	static toml::parse_result __hosts_table = toml::parse(secrets::hosts_toml);
	return __hosts_table;
}
