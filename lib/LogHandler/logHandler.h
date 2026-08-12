// clang-format off
#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#pragma once

/* LOG OPTIONS (also defineable trough compile flags):
 * WHEN LOGGING IS ENABLED THE MEMORY USED WILL INCREMENT
 * 
 * LOG: when defined enables logging
 * LOG_DEBUG: when defined will print log messages of level "DEBUG"
 * LOG_INFOS: when defined will print log messages of level "INFO"
 * LOG_WARNINGS: when defined will print log messages of level "WARNING"
 * LOG_ERRORS: when defined will print log messages of level "ERROR"
*/

// the defines are in the platformio.ini
// #define LOG
// #define LOG_DEBUG
// #define LOG_INFOS
// #define LOG_WARNINGS
// #define LOG_ERRORS

#ifdef LOG
	#include <Arduino.h>
	#include <errno.h>
	#include <stdarg.h>
	#include <stdio.h>


	#define MESSAGE_BUFFER_SIZE 1024
	#define FUNCTION_NAME_SIZE 32

	#define printDebugMessage(...) __printLogMessage(__FILE__, __func__, __LINE__, 'd', __VA_ARGS__)
	#define printInfoMessage(...) __printLogMessage(__FILE__, __func__, __LINE__, 'i', __VA_ARGS__)
	#define printWarningMessage(...) __printLogMessage(__FILE__, __func__, __LINE__, 'w', __VA_ARGS__)
	#define printErrorMessage(...) __printLogMessage(__FILE__, __func__, __LINE__, 'e', __VA_ARGS__)

	void __printLogMessage(const char* file, const char* function, int line, const char log_level, bool send_to_client, const char* format, ...);
	inline void __printLogMessage(const char* file, const char* function, int line, const char log_level, const char* format) {
		__printLogMessage(file, function, line, log_level, false, format);
	}
	inline void __printLogMessage(const char* file, const char* function, int line, const char log_level, const char* format, bool send_to_client) {
		__printLogMessage(file, function, line, log_level, send_to_client, format);
	}
	template <typename T, typename... Args>
	inline void __printLogMessage(const char* file, const char* function, int line, const char log_level, const char* format, T arg, Args... args) {
		__printLogMessage(file, function, line, log_level, false, format, arg, args...);
	}

	void __printLogMessage(const char* file, const char* function, int line, const char log_level, bool send_to_client, const char* format, ...);

using LogClientSendFn = void (*)(void* client, const char* message);
void setLogClient(void* client, LogClientSendFn send_fn);
void clearLogClient();
#else
	#define printDebugMessage(format, ...) do {} while (0);
	#define printInfoMessage(format, ...) do {} while (0);
	#define printWarningMessage(format, ...) do {} while (0);
	#define printErrorMessage(format, ...) do {} while (0);
#endif
#endif
// clang-format on
