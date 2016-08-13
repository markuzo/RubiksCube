#include "stdafx.h"
#include "Utilities.h"

#include <fstream>
#include <sstream>
#include <exception>

const char * 
Utilities::convertFileToConstChar(const char * filePath) {
	std::ifstream file;
	file.open(filePath);

	const char * returnData;

	if (!file.is_open()) {
		std::stringstream errorMessageStream = std::stringstream();
		errorMessageStream << "File " << filePath << " could not be opened." << std::endl;

		std::string errorMessage = errorMessageStream.str();
		
		throw std::exception(errorMessage.c_str());
	}
	else {
		std::stringstream data = std::stringstream();
		std::string line;
		while (getline(file, line)) {
			data << line << '\n';
		}
		
		std::string dataString = data.str();
		returnData = dataString.c_str();
	}
	return returnData;
}

std::string
Utilities::convertFileToString(const char * filePath) {
	std::ifstream file;
	file.open(filePath);

	std::string returnData;

	if (!file.is_open()) {
		std::stringstream errorMessageStream = std::stringstream();
		errorMessageStream << "File " << filePath << " could not be opened." << std::endl;

		std::string errorMessage = errorMessageStream.str();

		throw std::exception(errorMessage.c_str());
	}
	else {
		std::stringstream data = std::stringstream();
		std::string line;
		while (getline(file, line)) {
			data << line << '\n';
		}

		returnData = data.str();
	}
	return returnData;
}