// clang-format off
#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#pragma once

/* LOG OPTIONS (also defineable trough compile flags):
 * WHEN LOGGING IS ENABLED THE MEMORY USED WILL INCREMENT
 * 
 * LOG: when defined enables logging
 * LOG_INFOS: when defined will print log messages of level "INFO"
 * LOG_WARNINGS: when defined will print log messages of level "WARNING"
 * LOG_ERRORS: when defined will print log messages of level "ERROR"
*/
// #define LOG
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

	/*
	* __FILE__: complete path to the file that called the function where it's specified
	* __func__: name of the function that called the function where it's specified
	* __LINE__: line of the file where the function was called
	*/

	#ifdef LOG_INFOS
		#define printInfoMessage(format, ...) __printLogMessage(__FILE__, __func__, __LINE__, 'i', format, ##__VA_ARGS__);
	#else
		#define printInfoMessage(format, ...) do {} while (0);
	#endif

	#ifdef LOG_WARNINGS
		#define printWarningMessage(format, ...) __printLogMessage(__FILE__, __func__, __LINE__, 'w', format, ##__VA_ARGS__);
	#else
		#define printWarningMessage(format, ...) do {} while (0);
	#endif

	#ifdef LOG_ERRORS
		#define printErrorMessage(format, ...) __printLogMessage(__FILE__, __func__, __LINE__, 'e', format, ##__VA_ARGS__);
	#else
		#define printErrorMessage(format, ...) do {} while (0);
	#endif

	void __printLogMessage(const char* file, const char* function, int line, const char log_level, const char* format, ...);
#else
	#define printInfoMessage(format, ...) do {} while (0);
	#define printWarningMessage(format, ...) do {} while (0);
	#define printErrorMessage(format, ...) do {} while (0);
#endif
#endif
// clang-format on
