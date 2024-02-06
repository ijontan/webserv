
#include "webserv.h"

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

class Parser;
class ServerBlock;
class IOAdaptor;

class WebServer {
	public:
		WebServer( const std::string& filePath );
		~WebServer();

		// utils
		void	printServerBlocksInfo();
		void	initSockets();
		void	loop(IOAdaptor io);
		void	addPfd(int fd);
		void	removePfd(int index);

	private:
		WebServer( const WebServer& other );
		WebServer& operator=( const WebServer& other );

		std::vector<ServerBlock>	_serverBlocks;
		std::vector<struct pollfd>	pfds;
};

#endif
