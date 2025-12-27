#include <Host.h>
#include <iostream>
#include <logHandler.h>
#include <utils.h>

Host::Host(
    std::string name,
    std::string type,
    bool use_relay,
    int relay_pin,
    bool use_magic_packet,
    std::string mac_address) {
	this->__name = name;
	this->__type = type;
	this->__use_relay = use_relay;
	this->__relay_pin = relay_pin;
	this->__use_magic_packet = use_magic_packet;
	try {
		this->__mac_address = utils::convertStringVectorToByteVector(utils::split(mac_address, ':'));
	} catch (const std::exception& e) {
		printErrorMessage(e.what());
	}
}

std::string Host::getName() {
	return this->__name;
}

std::string Host::getType() {
	return this->__type;
}

bool Host::isUseRelayPinEnabled() {
	return this->__use_relay;
}

int Host::getRelayPin() {
	return this->__relay_pin;
}

bool Host::isUseMagicPacketEnabled() {
	return this->__use_magic_packet;
}

std::vector<byte> Host::getMacAddress() {
	return this->__mac_address;
}
