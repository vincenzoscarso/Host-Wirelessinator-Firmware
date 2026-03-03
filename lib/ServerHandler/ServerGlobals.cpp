#include "serverGlobals.h"

ServerGlobals::ServerGlobals()
    : keep_connection(false)
    , is_client_connected(false)
    , is_request_being_handled(false)
    , last_client_activity(0) { }

bool ServerGlobals::isKeepConnectionEnabled() const {
	return keep_connection;
}

void ServerGlobals::setKeepConnection(bool keep) {
	keep_connection = keep;
}

bool ServerGlobals::isRequestBeingHandled() const {
	return is_request_being_handled;
}

void ServerGlobals::setIsRequestBeingHandled(bool handled) {
	is_request_being_handled = handled;
}

/*==========
    SERVER
  ==========*/

bool ServerGlobals::isServerAvailable() {
	return server.available();
}

websockets::WebsocketsServer& ServerGlobals::getServer() {
	return server;
}

bool ServerGlobals::serverListen(uint16_t port) {
	server.listen(port);
	return server.available();
}

/*==========
    CLIENT
  ==========*/

websockets::WebsocketsClient& ServerGlobals::getClient() {
	return client;
}

void ServerGlobals::setClient(const websockets::WebsocketsClient& newClient) {
	client = newClient;
}

/*---------------------
    client-connection
  ---------------------*/

bool ServerGlobals::isClientConnected() const {
	return is_client_connected;
}

void ServerGlobals::setClientConnected(bool connected) {
	is_client_connected = connected;
}

/*-------------------
    client-activity
  -------------------*/

unsigned long ServerGlobals::getLastClientActivity() const {
	return last_client_activity;
}

void ServerGlobals::updateLastClientActivity() {
	last_client_activity = millis();
}

bool ServerGlobals::isClientTimedOut() const {
	unsigned long now = millis();
	return (now - last_client_activity > CLIENT_TIMEOUT_MS);
}
