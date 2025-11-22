#include "logHandler.h"

#ifdef LOG

static void __printHeader(const char* file, const char* function, int line, int log_level);
static void __printInfoHeader(const char* function);
static void __printWarningHeader(const char* function);
static void __printErrorHeader(const char* file, const char* function, int line);
static void __shortenFuntionName(char* function_name_buffer, const char* function);

void __printLogMessage(const char* file, const char* function, int line, const char log_level, const char* format, ...) {
	const char truncated_message_notice[] = "...[some characters got truncated]";
	char buffer[MESSAGE_BUFFER_SIZE];
	va_list p_arguments;

	va_start(p_arguments, format);
	errno = 0;
	/* vsnprintf():
	 * PER RICORDARE (documentazione vsnprintf():
	 * https://learn.microsoft.com/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l):
	 * - sizeOfBuffer: grandezza effettiva del buffer
	 * - count (secondo parametro di vsnprintf): numero massimo di caratteri che
	 * vsnprintf può scrivere
	 * - len (riferito al format fornito): numero di carateri che DOVREBBERO
	 * essere scritti
	 */
	int result = vsnprintf(buffer, MESSAGE_BUFFER_SIZE - sizeof(truncated_message_notice), format, p_arguments);
	va_end(p_arguments);

	if (result > 0 && result < MESSAGE_BUFFER_SIZE) { // Everything written correctly
		__printHeader(file, function, line, log_level);
		Serial.println(buffer);

	} else if (result > 0 && result >= MESSAGE_BUFFER_SIZE) { // Truncated some characters
		__printHeader(file, function, line, log_level);
		Serial.print(buffer);
		Serial.println(truncated_message_notice);

	} else if (result < 0) { // Error (value in errno)
		printErrorMessage("vsnprintf() returned %d, failed the fetching of a message (errno: %d)", result, errno);
	}
}

void __printHeader(const char* file, const char* function, int line, int log_level) {
	switch (log_level) {
	case 'i':
		__printInfoHeader(function);
		break;
	case 'w':
		__printWarningHeader(function);
		break;
	case 'e':
		__printErrorHeader(file, function, line);
		break;
	}
}

#ifdef LOG_INFOS
void __printInfoHeader(const char* function) {
	int size_of_function_name = strlen(function);

	if (size_of_function_name > FUNCTION_NAME_SIZE) {
		char function_name_buffer[FUNCTION_NAME_SIZE];
		__shortenFuntionName(function_name_buffer, function);
		Serial.printf("[INFO   ][%s...]: ", function_name_buffer);
	} else {
		Serial.printf("[INFO   ][%+*s]: ", FUNCTION_NAME_SIZE, function);
	}
}

#else /* LOG_INFOS */
void __printInfoHeader(const char* function) {
	do {
	} while (0);
}
#endif /* LOG_INFOS */

#ifdef LOG_WARNINGS
void __printWarningHeader(const char* function) {
	int size_of_function_name = strlen(function);

	if (size_of_function_name > FUNCTION_NAME_SIZE) {
		char function_name_buffer[FUNCTION_NAME_SIZE];
		__shortenFuntionName(function_name_buffer, function);
		Serial.printf("[WARNING][%s...]: ", function_name_buffer);
	} else {
		Serial.printf("[WARNING][%+*s]: ", FUNCTION_NAME_SIZE, function);
	}
}
#else /* LOG_WARNINGS */
void __printWarningHeader(const char* function) {
	do {
	} while (0);
}
#endif /* LOG_WARNINGS */

#ifdef LOG_ERRORS
void __printErrorHeader(const char* file, const char* function, int line) {
	Serial.printf("[ERROR  ][%s:%d][%s]:", file, line, function);
	Serial.print("\n   \\---> ");
}
#else /* LOG_ERRORS */
void __printErrorHeader(const char* file, const char* function, int line) {
	do {
	} while (0);
}
#endif /* LOG_ERRORS */

void __shortenFuntionName(char* function_name_buffer, const char* function) {
	if (function_name_buffer == NULL || function == NULL || FUNCTION_NAME_SIZE == 0) {
		if (function_name_buffer != NULL && FUNCTION_NAME_SIZE > 0) {
			function_name_buffer[0] = '\0';
		}
		printErrorMessage("shortening of a function name failed");
		return;
	}

	strncpy(function_name_buffer, function, FUNCTION_NAME_SIZE - 3);
	function_name_buffer[FUNCTION_NAME_SIZE - 3] = '\0'; // -3 beacuse of "..."
}

#endif /* LOG */
