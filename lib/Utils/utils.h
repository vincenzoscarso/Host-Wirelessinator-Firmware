#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>

namespace utils {
std::string toStdString(const char* string);
std::string toStdString(const String& string);
std::vector<std::string> split(std::string& str, char delimiter);
std::vector<byte> convertStringVectorToByteVector(std::vector<std::string> string_vector);
} // namespace utils

#endif /* UTILS_H */