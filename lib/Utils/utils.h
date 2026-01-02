#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>

namespace utils {
std::string toStdString(const char* string);
std::string toStdString(const String& string);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::string trim(const std::string& str);
std::vector<byte> convertStringVectorToByteVector(std::vector<std::string> string_vector);
int getLineNumberOfString(const std::string& container_string, const std::string& string_to_find);
std::string getLine(const std::string& str, int line);
} // namespace utils

#endif /* UTILS_H */