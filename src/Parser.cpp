
#include "webserv.h"

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser( const Parser& other ) {
	(void)other;
}

Parser& Parser::operator=( const Parser& other ) {
	(void)other;
	return *this;
}

std::vector<ServerBlock> Parser::createServerBlocks( std::string filePath ) {
	// open the config file
	std::ifstream	file(filePath);

	if (file.is_open() == false) {
		throw CustomException("Error: Cannot open config file");
	}

	// parsing the server blocks
	std::string 				line;
	int							lineNum = 1;
	std::vector<ServerBlock>	serverBlocks;

	while (std::getline(file, line)) {
		// skips empty lines and comments
		if (line.empty() || line.find('#') == 0) {
			lineNum++;
			continue;
		}

		// converts the line into a input string stream
		std::istringstream 	iss(line);
		std::string			directive1, directive2;
		
		// gets the 1st and 2nd element in the line
		iss >> directive1 >> directive2;
		std::cout << "directive1: " << directive1 << std::endl;
		std::cout << "directive2: " << directive2 << std::endl;

		if (directive1 == "server" && directive2 == "{") {
			ServerBlock serverBlock;

			lineNum++;
			while (std::getline(file, line)) {
				if (line.empty() || line.find('#') == 0) {
					lineNum++;
					continue;
				}
				std::istringstream 	iss(line);
				std::string			directive;

				iss >> directive;
				if (directive == "listen") {
					std::string port;
					iss >> port;
					std::cout << port << std::endl;
				}
				lineNum++;
			}
		} else {
			std::string errorMsg = "Error at line "
									+ std::to_string(lineNum)
									+ ": Server blocks must be in the enclosed in this format:\nserver {\n...\n}";
			throw CustomException(errorMsg);
		}
	}	
	return (serverBlocks);
}
