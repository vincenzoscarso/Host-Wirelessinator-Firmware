#include "tiny_websockets/internals/websockets_endpoint.hpp"
#include <ArduinoWebsockets.h>
#include <Hosts.h>
#include <ServerGlobals.h>
#include <WiFi.h>
#include <commandHandler.h>
#include <componentHandler.h>
#include <serverHandler.h>
#include <sys/unistd.h>
#include <utils.h>

static ServerGlobals __globals;

static void __acceptNewConnections();
static void __rejectNewConnections();
static void __handleClientRequests(websockets::WebsocketsClient& client, websockets::WebsocketsMessage raw_request);
static void __handleConnectedClientConnection();
static bool __updateConnectionMode(const std::string& message);
static void __handleCommand(websockets::WebsocketsClient& client, const std::string& message);

void ServerHandler::serverSetup() {
	printInfoMessage("Starting serverSetup procedure...");

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		printErrorMessage("Server setup canceled: no wifi connection");
		return;
	}

	printInfoMessage("Starting server...");

	if (!__globals.serverListen(SERVER_PORT)) {
		printInfoMessage("Failed to start the server");
		while (1)
			componentHandler::blinkLedBuiltIn(componentHandler::BLINK_RIPETITIONS_ON_SERVER_STARTUP_FAILURE);
	}
	printInfoMessage("The server is online");

	printInfoMessage("serverSetup procedure ended");
}

void ServerHandler::serverLoop() {
	if (!__globals.isServerAvailable()) {
		printErrorMessage("WebSockets' server is not live");
		while (1)
			componentHandler::blinkLedBuiltIn(componentHandler::BLINK_RIPETITIONS_ON_SERVER_STARTUP_FAILURE);
	}

	if (!__globals.isClientConnected()) {
		__acceptNewConnections();
	} else {
		__rejectNewConnections();
	}

	if (__globals.isClientConnected()) {
		__handleConnectedClientConnection();
	} 
}

static void __acceptNewConnections() {
	websockets::WebsocketsClient new_client;

	if (!__globals.getServer().poll()) {
		return;
	}

	new_client = __globals.getServer().accept();

	__globals.setClient(new_client);
	__globals.setClientConnected(true);
	__globals.setKeepConnection(false);
	__globals.updateLastClientActivity();

	__globals.getClient().onMessage(__handleClientRequests);

	setLogClient(&__globals.getClient(), [](void* client, const char* message) {
		static_cast<websockets::WebsocketsClient*>(client)->send(message);
	});

	printInfoMessage("Client accepted %s");
}

static void __rejectNewConnections() {
	if (!__globals.getServer().poll()) {
		return;
	}

	auto client_to_reject = __globals.getServer().accept();

	printInfoMessage("Received new connection request but socket is currently busy: rejecting connection");
	client_to_reject.send("Socket is busy, rejecting connection");
	client_to_reject.close();
}

static void __handleClientRequests(websockets::WebsocketsClient& client, websockets::WebsocketsMessage raw_request) {
	__globals.setIsRequestBeingHandled(true);

	if (!raw_request.isText()) {
		printErrorMessage(true, "Message received is not text, skipping");
		__globals.setIsRequestBeingHandled(false);
		return;
	}

	std::string str_request = utils::toStdString(raw_request.c_str());
	printInfoMessage(true, "Got request:\n%s", str_request.c_str());

	if (__updateConnectionMode(str_request) != true) {
		printErrorMessage(true, "Request ignored: Missing foundamental headers");
		__globals.setIsRequestBeingHandled(false);
		return;
	}

	auto parts = utils::split(str_request, "-- HEADER END --\n");

	if (parts.size() < 2) {
		printErrorMessage(true, "Request ignored: no body after header delimiter");
		__globals.setIsRequestBeingHandled(false);
		return;
	}

	std::string command = utils::trim(parts[1]);

	printInfoMessage(true, "New request, got command: %s", command.c_str());
	componentHandler::blinkLedBuiltIn(componentHandler::BLINK_RIPETITIONS_ON_COMMAND);

	__handleCommand(client, command);

	__globals.updateLastClientActivity();
	__globals.setIsRequestBeingHandled(false);
}

static void __handleConnectedClientConnection() {
	bool keep_connection = __globals.isKeepConnectionEnabled();
	bool is_timed_out = __globals.isClientTimedOut();
	bool is_request_being_handled = __globals.isRequestBeingHandled();
	unsigned long last_activity = __globals.getLastClientActivity();
	unsigned long now = millis();

	__globals.getClient().poll();

	if ((now - last_activity) < __globals.MS_TO_SEND_A_COMMAND_AFTER_CONNECTION) {
		return;
	}

	if (is_request_being_handled) {
		return;
	}

	if (!is_timed_out && keep_connection) {
		return;
	}

	printInfoMessage(true, "Closing client connection: %s", is_timed_out ? "timeout" : "request timeout");
	clearLogClient();
	__globals.getClient().close(websockets::CloseReason_NormalClosure);
	__globals.setClientConnected(false);
}

static bool __updateConnectionMode(const std::string& message) {
	int line_number = utils::getLineNumberOfString(message, "Connection");

	if (line_number == std::string::npos) {
		printErrorMessage(true, "Invalid request: 'Connection' header not found");
		return false;
	}

	auto connection_property = utils::getLine(message, line_number);
	auto splitted_connection_property = utils::split(connection_property, ":");

	if (splitted_connection_property.size() < 2) {
		printErrorMessage(true, "Invalid request: Malformed 'Connection' header");
		return false;
	}

	std::string connection_property_value = utils::trim(splitted_connection_property[1]);

	bool desired_connection = false;

	if (connection_property_value == "keep_connection") {
		desired_connection = true;
	} else if (connection_property_value == "close_connection") {
		desired_connection = false;
	} else {
		printErrorMessage(true, "Inexistent connection property value, defaulting to false");
		desired_connection = false;
	}

	__globals.setKeepConnection(desired_connection);

	return true;
}

static void __handleCommand(websockets::WebsocketsClient& client, const std::string& message) {
	std::string command_name = "";
	std::string host_name = "";

	std::istringstream string_stream(message);
	string_stream >> command_name >> host_name;

	if (!commandHandler::checkForCommandAndExcecute(client, command_name, host_name)) {
		commandHandler::handleNoCommand(client, message);
	}
}
