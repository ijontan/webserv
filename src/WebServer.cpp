
#include "webserv.h"

WebServer::WebServer( std::string filePath ) {
	try {

		Parser parser;

		std::ifstream 	file( filePath );
		std::string 	line;

		ServerBlock 	serverBlock;

		while (std::getline(file, line)) {
			
			// skips empty lines and comments
			if (line.empty() || line.find('#') == 0) {
				continue;
			}

			//
			std::istringstream 	iss(line);
			std::string			directive;
			iss >> directive;

			std::cout << "directive: " << directive << std::endl;
		}
		std::cout << GREEN "Server Block created" RESET << std::endl;
	} 
	catch (const std::exception& e)
	{
		std::cout << URED << e << RESET << std::endl;
	}
}

WebServer::~WebServer() {}

WebServer::WebServer( const WebServer& other ) {
	(void)other;
}

WebServer& WebServer::operator=( const WebServer& other ) {
	(void)other;
	return *this;
}

