#include <ArduinoWebsockets.h>
#include <Hosts.h>
#include <WiFi.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <serverHandler.h>
#include <utils.h>

websockets::WebsocketsServer __server;
websockets::WebsocketsClient __client;
bool __keep_connection = false;
bool __connected_client = false;
unsigned long __last_client_activity = 0UL;
const unsigned long __CLIENT_TIMEOUT_MS = 60UL * 1000UL;

void __handleClientRequestIfExists(websockets::WebsocketsClient& client);
void __updateConnectionMode(const std::string& message);
void __handleCommand(websockets::WebsocketsClient& client, const std::string& message);

void serverSetup() {
	printInfoMessage("Starting serverSetup procedure...");

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		printErrorMessage("Server setup canceled: no wifi connection");
		return;
	}

	printInfoMessage("Starting server...");

	__server.listen(SERVER_PORT);

	if (!__server.available()) {

		printInfoMessage("Failed to start the server");
		while (1)
			componentHandler::blinkLedBuiltIn(1);
	}

	printInfoMessage("The server is online");
	componentHandler::setLedBuiltInStatus(HIGH);

	printInfoMessage("serverSetup procedure ended");
}

void serverLoop() {
	if (!__connected_client) {
		if (__server.available()) {
			__client = __server.accept();
			__connected_client = true;
			__keep_connection = false;
			__last_client_activity = millis();
			printInfoMessage("Client accepted");
		}
	} else if (__connected_client && __server.available()) {
		auto new_client = __server.accept();
		new_client.send("Socket is busy, connection rejected");
		new_client.close();
		printInfoMessage("Another client tried to connect but socket was busy, client rejected");
	}

	if (__connected_client) {
		__handleClientRequestIfExists(__client);
	}

	if (__connected_client) {
		unsigned long now = millis();
		bool is_timed_out = (now - __last_client_activity > __CLIENT_TIMEOUT_MS);

		if (!__keep_connection || is_timed_out) {
			__client.poll();

			printInfoMessage("DEBUG: %s, timeout:%d, act.:%d", is_timed_out ? "YES RICO KABOOOM" : "no :(", __CLIENT_TIMEOUT_MS, __last_client_activity);

			printInfoMessage("Closing client connection: %s", is_timed_out ? "timeout" : "close requested");
			__client.close();
			__connected_client = false;
		} else {
			__client.poll();
		}
	}
}

void __handleClientRequestIfExists(websockets::WebsocketsClient& client) {
	if (client.available()) {
		websockets::WebsocketsMessage raw_request = client.readBlocking();
		__last_client_activity = millis();

		if (!raw_request.isText()) {
			printErrorMessage("The message received from the client is not text, skipping");
			return;
		}

		std::string str_request = utils::toStdString(raw_request.c_str());
		printInfoMessage("Got request:\n%s", str_request.c_str());

		__updateConnectionMode(str_request);

		std::string command = utils::trim(utils::split(str_request, "-- HEADER END --\n")[1]);

		printInfoMessage("New request from a client, got command: %s", command.c_str());
		componentHandler::blinkLedBuiltIn(1);

		__handleCommand(client, command);
	}
}

void __updateConnectionMode(const std::string& message) {
	int line_number = utils::getLineNumberOfString(message, "Connection");
	auto connection_property = utils::getLine(message, line_number);
	auto splitted_connection_property = utils::split(connection_property, ":");
	std::string connection_property_value = utils::trim(splitted_connection_property[1]);

	if (connection_property_value == "keep_connection") {
		__keep_connection = true;
	} else if (connection_property_value == "close_connection") {
		__keep_connection = false;
	} else {
		printErrorMessage("Inexistent connection property value, defaulting to false");
		__keep_connection = false;
	}
}

void __handleCommand(websockets::WebsocketsClient& client, const std::string& message) {
	std::string command_name = "";
	std::string host_name = "";

	std::istringstream string_stream(message);
	string_stream >> command_name >> host_name;

	if (!commandHandler::checkForCommandAndExcecute(client, command_name, host_name)) {
		printErrorMessage("Unknown command: %s", message.c_str());
	}
}
