#include <Hosts.h>
#include <componentHandler.h>

void __pinsSetup();
void __hostsRelayPinsSetup();

void componentHandler::componentsSetup() {
	Hosts::updateHostsVector();
	__pinsSetup();
}

#ifdef LED_BUILTIN

void componentHandler::setLedBuiltInStatus(uint8_t value) {
	digitalWrite(LED_BUILTIN, value);
}

void componentHandler::blinkLedBuiltIn(int ripetitions) {
	int i = 0;
	while (i < ripetitions) {
		digitalWrite(LED_BUILTIN, LOW);
		delay(125);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(125);
		i++;
	}
}

#else /* LED_BUILTIN */

void componentHandler::setLedBuiltInStatus(uint8_t value) {
	return;
}

void bcomponentHandler::linkLedBuiltIn(int ripetitions) {
	return;
}

#endif /* LED_BUILTIN */

void componentHandler::setHostRelayPinStatus(Host host, byte value) {
	digitalWrite(host.getRelayPin(), value);
}

void __pinsSetup() {
	printInfoMessage("Starting pins setup procedure...");

#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
#endif /* LED_BUILTIN */

	__hostsRelayPinsSetup();

	printInfoMessage("Pins setup procedure ended");
}

void __hostsRelayPinsSetup() {
	for (Host h : Hosts::hosts) {
		int relay_pin = h.getRelayPin();
		if (h.isUseRelayPinEnabled() && relay_pin > -1) {
			pinMode(relay_pin, OUTPUT);
		}
	}
}
