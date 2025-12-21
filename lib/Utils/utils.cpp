#include <utils.h>

std::string utils::toStdString(const char* string) {
	return std::string(string);
}

std::string utils::toStdString(const String& string) {
	return std::string(string.c_str());
}