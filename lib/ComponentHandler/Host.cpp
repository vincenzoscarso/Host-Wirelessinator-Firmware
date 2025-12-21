#include <Host.h>
#include <iostream>

Host::Host(
    std::string name,
    std::string type,
    bool use_relay,
    int relay_pin,
    bool use_magic_packet,
    std::string ip_address) {
	this->__name = name;
	this->__type = type;
	this->__use_relay = use_relay;
	this->__relay_pin = relay_pin;
	this->__use_magic_packet = use_magic_packet;
	this->__ip_address = ip_address;
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

std::string Host::getIpAddress() {
	return this->__ip_address;
}
