#include <Arduino.h>
#include <WiFi.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <secrets.h>

#define SERIAL_BAUD (115200)
#define HOST_NAME "Host-Wirelessinator"
int wifi_status = WL_IDLE_STATUS;

void setup() {
	printInfoMessage("Starting setup procedure...");
	Serial.begin(SERIAL_BAUD);

	while (!Serial)
		;

	componentsSetup();

    WiFi.setHostname(HOST_NAME);	
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	wifi_status = WiFi.waitForConnectResult();
	while (wifi_status != WL_CONNECTED) {
		printErrorMessage("Couldn't get a wifi connection, wifi_status: %d", wifi_status);
		while (1)
			blinkLedBuiltIn(1);
		delay(1000);
	}

	printInfoMessage("Successfully connected to '%s' with IP address '%s'", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
	setLedBuiltInStatus(HIGH);
}

void loop() {
}


