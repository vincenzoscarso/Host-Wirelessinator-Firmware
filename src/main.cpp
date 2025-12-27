#include <Arduino.h>
#include <componentHandler.h>
#include <serverHandler.h>
#include <logHandler.h>
#include <wifiHandler.h>

#define SERIAL_BAUD (115200)

void setup() {
	Serial.begin(SERIAL_BAUD);
	while (!Serial)
		continue;

	printInfoMessage("Starting setup procedure...");

	componentsSetup();
	WifiHandler::wifiSetup();
	serverSetup();

	printInfoMessage("Setup procedure ended");
}

void loop() {
	serverLoop();
}
