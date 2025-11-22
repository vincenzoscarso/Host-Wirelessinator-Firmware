#include <componentHandler.h>

void __pinsSetup();

void componentsSetup() {
	__pinsSetup();
}

#ifdef LED_BUILTIN

void setLedBuiltInStatus(uint8_t value) {
	digitalWrite(LED_BUILTIN, value);
}

void blinkLedBuiltIn(int ripetitions) {
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

void setLedBuiltInStatus(uint8_t value) {
	return;
}

void blinkLedBuiltIn(int ripetitions) {
	return;
}

#endif /* LED_BUILTIN */

void __pinsSetup() {
	printInfoMessage("Starting pins setup procedure...");

#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
#endif /* LED_BUILTIN */

	printInfoMessage("Pins setup procedure ended");
}