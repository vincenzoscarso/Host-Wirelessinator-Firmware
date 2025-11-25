#include <WiFi.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <secrets.h>
#include <wifiHandler.h>

int __wifi_status = WL_IDLE_STATUS;

static void __handleClientRequest(WiFiClient client);
static String __getWifiStatus();

void wifiSetup() {
	printInfoMessage("Starting wifiSetup procedure...");
	WiFi.setHostname(HOST_NAME);

	do {
		WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
		__wifi_status = WiFi.waitForConnectResult();

		if (__wifi_status != WL_CONNECTED) {
			printErrorMessage("Couldn't get a WiFi connection, wifi_status: %d", __wifi_status);
			blinkLedBuiltIn(1);
			delay(DELAY_BETWEEN_WIFI_CONNECTION_TRIALS);
		}
	} while (__wifi_status != WL_CONNECTED);
	printInfoMessage("Successfully obtained a WiFi connection: {%s}", __getWifiStatus().c_str());
	setLedBuiltInStatus(HIGH);

	printInfoMessage("wifiSetup procedure ended");
}

String __getWifiStatus() {
	String wifi_status;

	wifi_status = "SSID: " + WiFi.SSID() + ", ";
	wifi_status += "IP Address: " + WiFi.localIP().toString() + ", ";
	wifi_status += "Signal strength (RSSI): " + String(WiFi.RSSI()) + " dBm";

	return wifi_status;
}
