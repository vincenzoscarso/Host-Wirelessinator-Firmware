#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>

namespace utils {
std::string toStdString(const char* string);
std::string toStdString(const String& string);
} // namespace utils

#endif /* UTILS_H */