#pragma once

#include <string>

class Utilities {
public:
	static std::string	convertFileToString(const char * filePath);
	static const char * convertFileToConstChar(const char * filePath);
};