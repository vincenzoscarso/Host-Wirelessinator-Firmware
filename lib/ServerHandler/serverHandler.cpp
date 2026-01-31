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
bool __is_client_connected = false;
bool __is_request_begin_handled_now = false;
unsigned long __last_client_activity = 0UL;
const unsigned long __CLIENT_TIMEOUT_MS = 10 * 1000UL; // 1000UL: 1000ms => 1s

void __acceptNewConnections();
void __rejectNewConnections();
void __handleClientRequests(websockets::WebsocketsClient& client, websockets::WebsocketsMessage raw_request);
void __handleConnectedClientConnection();
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
	bool is_server_still_available = __server.available();

	if (!__is_client_connected && is_server_still_available) {
		__acceptNewConnections();
	}

	else if (__is_client_connected && is_server_still_available) {
		//__rejectNewConnections(); //TODO: fix this function
	}

	else if (!is_server_still_available) {
		printErrorMessage("WebSockets' server is not live");
	}

	if (__is_client_connected) {
		__handleConnectedClientConnection();
	}
}

void __acceptNewConnections() {
	__client = __server.accept();

	__is_client_connected = true;
	__keep_connection = true;

	//__client.onEvent(); //TODO: TO SETUP IN FUTURE
	__client.onMessage(__handleClientRequests);

	__last_client_activity = millis();

	printInfoMessage("Client accepted");
}

void __rejectNewConnections() {
	auto client_to_reject = __server.accept();
	client_to_reject.send("Socket is busy, connection rejected");
	client_to_reject.close();
	printInfoMessage("Another client tried to connect but socket was busy, client rejected");
}

void __handleClientRequests(websockets::WebsocketsClient& client, websockets::WebsocketsMessage raw_request) {
	__is_request_begin_handled_now = true;
	
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

	__last_client_activity = millis();
	__is_request_begin_handled_now = false;
}

void __handleConnectedClientConnection() {
	unsigned long now = millis();
	bool is_timed_out = (now - __last_client_activity > __CLIENT_TIMEOUT_MS);

	if ((!__keep_connection || is_timed_out) 
		&& !__is_request_begin_handled_now) {
		__client.poll();

		printInfoMessage("Closing client connection: %s", is_timed_out ? "timeout" : "close requested");
		__client.close();
		__is_client_connected = false;
	} else {
		__client.poll();
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
		printErrorMessage("Inexistent connection property value, defaulting to true");
		__keep_connection = true;
	}
}

void __handleCommand(websockets::WebsocketsClient& client, const std::string& message) {
	std::string command_name = "";
	std::string host_name = "";

	std::istringstream string_stream(message);
	string_stream >> command_name >> host_name;

	if (!commandHandler::checkForCommandAndExcecute(client, command_name, host_name)) {
		commandHandler::handleNoCommand(client, message);
	}
}
