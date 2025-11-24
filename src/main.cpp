#include <Arduino.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <wifiHandler.h>

#define SERIAL_BAUD (115200)

void setup() {
	Serial.begin(SERIAL_BAUD);
	while (!Serial)
		;

	printInfoMessage("Starting setup procedure...");

	componentsSetup();
	wifiSetup();

	printInfoMessage("Setup procedure ended");
}

void loop() {
	wifiLoop();
}
