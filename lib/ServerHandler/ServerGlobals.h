#ifndef SERVERGLOBALS_H
#define SERVERGLOBALS_H

#include <Arduino.h>
#include <ArduinoWebsockets.h>

class ServerGlobals {
private:
	bool keep_connection;
	bool is_client_connected;
	bool is_request_being_handled;
	unsigned long last_client_activity;

	websockets::WebsocketsServer server;
	websockets::WebsocketsClient client;

public:
	static const unsigned long CLIENT_TIMEOUT_MS = 30UL * 1000UL;
	static const unsigned long MS_TO_SEND_A_COMMAND_AFTER_CONNECTION = 5000UL;
	
	ServerGlobals();

	bool isKeepConnectionEnabled() const;
	void setKeepConnection(bool keep_connection_preference);

	bool isRequestBeingHandled() const;
	void setIsRequestBeingHandled(bool handled);

	/*==========
	    SERVER
	  ==========*/

	bool isServerAvailable();
	websockets::WebsocketsServer& getServer();
	bool serverListen(uint16_t port);

	/*==========
	    CLIENT
	  ==========*/
	websockets::WebsocketsClient& getClient();
	void setClient(const websockets::WebsocketsClient& new_client);

	/*---------------------
	    client-connection
	  ---------------------*/
	bool isClientConnected() const;
	void setClientConnected(bool connected);

	/*-------------------
	    client-activity
	  -------------------*/
	unsigned long getLastClientActivity() const;
	void updateLastClientActivity();
	bool isClientTimedOut() const;
};

#endif /* SERVERGLOBALS_H */
