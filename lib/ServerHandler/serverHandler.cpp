#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <componentHandler.h>
#include <serverHandler.h>

using namespace websockets;

WebsocketsServer server;

void __handleClientRequest(WebsocketsClient client);

void serverSetup() {
	printInfoMessage("Starting serverSetup procedure...");

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		printErrorMessage("Server setup canceled: no wifi connection");
		return;
	}

	printInfoMessage("Starting server...");

	server.listen(SERVER_PORT);

	if (!server.available()) {

		printInfoMessage("Failed to start the server");
		while (1)
			blinkLedBuiltIn(1);
	}

	printInfoMessage("The server is online");
	setLedBuiltInStatus(HIGH);

	printInfoMessage("serverSetup procedure ended");
}

void serverLoop() {
	WebsocketsClient client = server.accept();
	__handleClientRequest(client);
	delay(1000);
}

void __handleClientRequest(WebsocketsClient client) {
	if (client.available()) {
		WebsocketsMessage message = client.readBlocking();
		printInfoMessage("New request from a client");
		blinkLedBuiltIn(1);

		printInfoMessage("Got Message:\n%s", message.data().c_str());

		client.send("Hello World!");

		client.close();
	}
}

bool __checkForCommand(const char* message) {
	/**
	 * Boot {host_id}
	 * Reboot {host_id}
	 * ForceShutdown {host_id}
	 * GetStatus {host_id}
	 * Informations
	 * Help
	 */

}