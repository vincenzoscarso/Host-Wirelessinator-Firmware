#include <WiFi.h>
#include <WiFiUdp.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <secrets.h>
#include <wifiHandler.h>

static void __connectToWifi();
static String __getWifiStatusString();

void WifiHandler::wifiSetup() {
	printInfoMessage("Starting wifiSetup procedure...");
	WiFi.setHostname(HOST_NAME);

	__connectToWifi();

	printInfoMessage("wifiSetup procedure ended");
}

void WifiHandler::wifiLoop() {
	static unsigned long last_connection_check = 0;

	if (millis() - last_connection_check >= DELAY_BETWEEN_WIFI_CONNECTION_CHECK) {
		if (WiFi.waitForConnectResult() != WL_CONNECTED) {
			printErrorMessage("Lost WiFi connection retrying to connect...")
			__connectToWifi();
		}

		last_connection_check = millis();
	}
}

void __connectToWifi() {
	uint64_t status;

	do {
		WiFi.begin(secrets::WIFI_SSID.c_str(), secrets::WIFI_PASSWORD.c_str());
		status = WiFi.waitForConnectResult();

		if (status != WL_CONNECTED) {
			printErrorMessage("Couldn't get a WiFi connection, wifi_status: %d", status);
			componentHandler::blinkLedBuiltIn(componentHandler::BLINK_RIPETITIONS_BETWEEN_FAILED_CONNECTION_TRIALS);
			delay(DELAY_BETWEEN_WIFI_CONNECTION_TRIALS);
		}
	} while (status != WL_CONNECTED);

	printInfoMessage("Successfully obtained a WiFi connection: {%s}", __getWifiStatusString().c_str());
}

String __getWifiStatusString() {
	String wifi_status;

	wifi_status = "SSID: " + WiFi.SSID() + ", ";
	wifi_status += "IP Address: " + WiFi.localIP().toString() + ", ";
	wifi_status += "Signal strength (RSSI): " + String(WiFi.RSSI()) + " dBm";

	return wifi_status;
}

void WifiHandler::sendMagicPacket(std::vector<byte> receiver_mac_address) {
	if (receiver_mac_address.size() != 6) {
		printErrorMessage("MAC address must be 6 bytes long.");
		return;
	}

	std::vector<byte> packet = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	for (int i = 0; i < 16; i++) {
		packet.insert(packet.end(), receiver_mac_address.begin(), receiver_mac_address.end());
	}

	WiFiUDP Udp;
	std::string broadcast_address = "255.255.255.255";
	int port = 9;

	Udp.beginPacket(broadcast_address.c_str(), port);
	Udp.write(packet.data(), packet.size());
	Udp.endPacket();
}
