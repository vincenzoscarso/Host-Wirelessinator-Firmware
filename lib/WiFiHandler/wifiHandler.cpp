#include <WiFi.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <secrets.h>
#include <wifiHandler.h>

int __wifi_status = WL_IDLE_STATUS;

static String __getWifiStatus();

void wifiSetup() {
	printInfoMessage("Starting wifiSetup procedure...");
	WiFi.setHostname(HOST_NAME);

	do {
		WiFi.begin(secrets::WIFI_SSID.c_str(), secrets::WIFI_PASSWORD.c_str());
		__wifi_status = WiFi.waitForConnectResult();

		if (__wifi_status != WL_CONNECTED) {
			printErrorMessage("Couldn't get a WiFi connection, wifi_status: %d", __wifi_status);
			blinkLedBuiltIn(1);
			delay(DELAY_BETWEEN_WIFI_CONNECTION_TRIALS);
		}
	} while (__wifi_status != WL_CONNECTED);
	printInfoMessage("Successfully obtained a WiFi connection: {%s}", __getWifiStatus().c_str());

	printInfoMessage("wifiSetup procedure ended");
}

String __getWifiStatus() {
	String wifi_status;

	wifi_status = "SSID: " + WiFi.SSID() + ", ";
	wifi_status += "IP Address: " + WiFi.localIP().toString() + ", ";
	wifi_status += "Signal strength (RSSI): " + String(WiFi.RSSI()) + " dBm";

	return wifi_status;
}
