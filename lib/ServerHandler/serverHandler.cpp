#include <ArduinoWebsockets.h>
#include <Hosts.h>
#include <WiFi.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <serverHandler.h>
#include <utils.h>

websockets::WebsocketsServer server;

void __handleClientRequest(websockets::WebsocketsClient client);
void __handleCommand(const std::string& message);

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
	websockets::WebsocketsClient client = server.accept();
	__handleClientRequest(client);
}

void __handleClientRequest(websockets::WebsocketsClient client) {
	if (client.available()) {
		websockets::WebsocketsMessage raw_message = client.readBlocking();

		if (!raw_message.isText()) {
			printErrorMessage("The message received from the client is not text, skipping");
			return;
		}

		std::string str_message = utils::toStdString(raw_message.c_str());

		printInfoMessage("New request from a client, got command: %s", str_message.c_str());
		blinkLedBuiltIn(1);

		__handleCommand(str_message);

		client.send("Hello World!");

		client.close();
	}
}

void __handleCommand(const std::string& message) {
	std::string command_name = "";
	std::string host_name = "";

	std::istringstream string_stream(message);
	string_stream >> command_name >> host_name;

	if (!commandHandler::checkForCommandAndExcecute(command_name, host_name)) {
		printErrorMessage("Unknown command: %s", message.c_str());
	}
}

