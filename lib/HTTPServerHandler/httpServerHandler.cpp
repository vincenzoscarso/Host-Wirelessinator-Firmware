#include <WiFi.h>
#include <componentHandler.h>
#include <httpServerHandler.h>
#include <logHandler.h>

String header = R"(HTTP/1.1 200 OK
Content-Type: text/html
Connection: close
Content-Length: )";

String response = R"(<!DOCTYPE HTML>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
    </head>
    <body>
        Hello World!
    </body>
</html>)";

WiFiServer server(HTTP_SERVER_PORT);

void __handleClientRequest(WiFiClient client);

void httpServerSetup() {
	printInfoMessage("Starting httpServerSetup procedure...");

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		printErrorMessage("HTTP server setup canceled: no wifi connection");
		return;
	}

	printInfoMessage("Starting HTTP server...");
	server.begin();

	printInfoMessage("httpServerSetup procedure ended");
}

void httpServerLoop() {
	WiFiClient client = server.available();

	if (client) {
		__handleClientRequest(client);
	}
}

void __handleClientRequest(WiFiClient client) {
	printInfoMessage("New client '%s'", client.localIP().toString().c_str());
	blinkLedBuiltIn(1);

	String request = "";
	bool currentLineIsBlank = true;

	while (client.connected()) {
		if (client.available()) {
			char c = client.read();
			request += String(c);

			if (c == '\n' && currentLineIsBlank) {
				printInfoMessage("New request:\n%s", request.c_str());
				String content_length = String(response.length());
				String final_response = String(header + content_length + "\r\n\r\n" + response);
				printInfoMessage("Sending response:\n%s", final_response.c_str());
				client.print(final_response);
				break;
			}

			if (c == '\n') {
				currentLineIsBlank = true;
			} else if (c != '\r') {
				currentLineIsBlank = false;
			}
		}
	}

	delay(1); // Time for the web browser to receive the data
	client.stop();
	printInfoMessage("Client disconnected");
}