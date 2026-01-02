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

std::vector<std::string> utils::split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t token_start_pos = 0;
	size_t token_end_pos = str.find(delimiter);

	while (token_end_pos != std::string::npos) {

		if (token_end_pos != token_start_pos) {
			tokens.push_back(str.substr(token_start_pos, token_end_pos - token_start_pos));
		}
		token_start_pos = token_end_pos + delimiter.length();
		token_end_pos = str.find(delimiter, token_start_pos);
	}

	if (token_start_pos < str.length()) {
		tokens.push_back(str.substr(token_start_pos));
	}

	return tokens;
}

std::string utils::trim(const std::string& str) {
    const std::string characters_to_trim = " \t\n\r\f\v";
    
    size_t first = str.find_first_not_of(characters_to_trim);

    if (first == std::string::npos) return "";

    size_t last = str.find_last_not_of(characters_to_trim);
    
    return str.substr(first, (last - first + 1));
}

int utils::getLineNumberOfString(const std::string& container_string, const std::string& string_to_find) {
	size_t position = container_string.find(string_to_find);
	int i = 0;
	int line = 0;

	for (char ch : container_string) {
		if (ch == '\n' && i < position) {
			line++;
		} else if (i >= position) {
			break;
		}
		i++;
	}

	return line;
}

/**
 * @param str: the string of the line to extract
 * @param line: the line to extract from the string (starting from 0)
 */
std::string utils::getLine(const std::string& str, int line) {
	if (!str.find('\n')) {
		throw std::invalid_argument("No lines found in this string");
	}

	auto splitted_string = utils::split(str, "\n");

	return splitted_string[line];
}
