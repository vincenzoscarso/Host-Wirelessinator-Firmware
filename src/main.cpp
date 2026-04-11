#include <Arduino.h>
#include <componentHandler.h>
#include <logHandler.h>
#include <serverHandler.h>
#include <wifiHandler.h>

#define SERIAL_BAUD (115200)

void __printInfos();

#ifdef LOG_INFOS

void __printLoopsPerformed();
void __printTasksInfos();

#else /* LOG_INFOS */

// clang-format off
#define __printLoopsPerformed() do {} while (0)
#define __printTasksInfos()     do {} while (0)
// clang-format on

#endif /* LOG_INFOS */

void setup() {
	Serial.begin(SERIAL_BAUD);

	printInfoMessage("Starting setup procedure...");

	componentHandler::componentsSetup();
	WifiHandler::wifiSetup();
	ServerHandler::serverSetup();

	printInfoMessage("Setup procedure ended");
}

void loop() {
	__printInfos();
	WifiHandler::wifiLoop();
	ServerHandler::serverLoop();
}

void __printInfos() {
	__printLoopsPerformed();
	//__printTasksInfos();
}

#ifdef LOG_INFOS

void __printLoopsPerformed() {
	static int cycles = 0;
	static unsigned long last_info_print = 0;
	const unsigned long millis_between_info_prints = 1000;
	const unsigned long int time_passed_since_last_print = millis() - last_info_print;

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
	const unsigned long int time_passed_since_last_print = millis() - last_info_print;

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
#else /* LOG_INFOS */

// clang-format off
#define __printLoopsPerformed() do {} while (0)
#define __printTasksInfos() do {} while (0)
// clang-format on

#endif /* LOG_INFOS */
