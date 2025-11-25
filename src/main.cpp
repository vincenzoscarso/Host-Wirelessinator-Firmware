#include <Arduino.h>
#include <componentHandler.h>
#include <httpServerHandler.h>
#include <logHandler.h>
#include <wifiHandler.h>

#define SERIAL_BAUD (115200)

void setup() {
	Serial.begin(SERIAL_BAUD);
	while (!Serial)
		continue;

	printInfoMessage("Starting setup procedure...");

	componentsSetup();
	wifiSetup();
	httpServerSetup();

	printInfoMessage("Setup procedure ended");
}

void loop() {
	httpServerLoop();
}
