#include <logHandler.h>
#include <stdexcept>
#include <utils.h>

std::string utils::toStdString(const char* string) {
	return std::string(string);
}

std::string utils::toStdString(const String& string) {
	return std::string(string.c_str());
}

std::vector<byte> utils::convertStringVectorToByteVector(std::vector<std::string> string_vector) {
	std::vector<byte> byte_vector;

	for (std::string string : string_vector) {
		int value = std::stoi(string, nullptr, 16);

		if (value < 0 || value > 255) {
			throw std::invalid_argument("Can't convert string to byte value: value out of range");
		}

		byte_vector.push_back(static_cast<byte>(value));
	}

	return byte_vector;
}

std::vector<std::string> utils::split(std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;

	for (char ch : str) {
		if (ch == delimiter) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else {
			token += ch;
		}
	}

	if (!token.empty()) {
		tokens.push_back(token);
	}

	return tokens;
}