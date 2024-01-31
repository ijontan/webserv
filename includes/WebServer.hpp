
#include "webserv.h"

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

class Parser;
class ServerBlock;

class WebServer {
	public:
		WebServer( const std::string& filePath );
		~WebServer();

	private:
		WebServer( const WebServer& other );
		WebServer& operator=( const WebServer& other );

		std::vector<ServerBlock>	_serverBlocks;
};

#endif
