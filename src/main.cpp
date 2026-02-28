#include <Arduino.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <serverHandler.h>
#include <wifiHandler.h>

#define SERIAL_BAUD (115200)

void __printInfos();
void __printLoopsPerformed();
void __printTasksInfos();

void setup() {
	Serial.begin(SERIAL_BAUD);

	printInfoMessage("Starting setup procedure...");

	componentHandler::componentsSetup();
	WifiHandler::wifiSetup();
	serverSetup();

	printInfoMessage("Setup procedure ended");
}

void loop() {
	__printInfos();
	serverLoop();
}

void __printInfos() {
	__printLoopsPerformed();
	//__printTasksInfos();
}

void __printLoopsPerformed() {
	static int cycles = 0;
	static unsigned long last_info_print = 0;
	const unsigned long millis_between_info_prints = 1000;
	const int time_passed_since_last_print = millis() - last_info_print;

	if (time_passed_since_last_print >= millis_between_info_prints) {
		printInfoMessage("Performed %d cycle(s) in the last %ldms", cycles, time_passed_since_last_print);
		last_info_print = millis();
		cycles = 0;
	}

	cycles++;
}

void __printTasksInfos() {
	static unsigned long last_info_print = 0;
	const unsigned long millis_between_info_prints = 1000;
	const int time_passed_since_last_print = millis() - last_info_print;

	if (time_passed_since_last_print < millis_between_info_prints)
		return;

	char taskListBuffer[1024];
	vTaskList(taskListBuffer);

	Serial.println("=====================");
	Serial.println("  All Running Tasks");
	Serial.println("=====================");
	Serial.printf("%s\n", taskListBuffer);

	last_info_print = millis();
}
