#ifndef HOST_H
#define HOST_H

#include <Arduino.h>
#include <string>

class Host {
private:
	std::string __name;
	std::string __type;
	bool __use_relay;
	int __relay_pin;
	bool __use_magic_packet;
	std::vector<byte> __mac_address;

public:
	Host(
	    std::string name,
	    std::string type,
	    bool use_relay,
	    int relay_pin,
	    bool use_magic_packet,
	    std::string mac_address);

	std::string getName();
	std::string getType();
	bool isUseRelayPinEnabled();
	int getRelayPin();
	bool isUseMagicPacketEnabled();
	std::vector<byte> getMacAddress();
};

#endif /* HOST_H */