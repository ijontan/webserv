
#include "webserv.h"

WebServer::WebServer(const std::string &filePath)
{
	try
	{
		Parser parser(filePath);

		parser.parseServerBlocks(this->_serverBlocks);
		std::cout << GREEN "Server blocks created" RESET << std::endl
				  << std::endl;

		printServerBlocksInfo();
	}
	catch (const std::exception &e)
	{
		std::cout << BRED << e.what() << RESET << std::endl;
	}
}

WebServer::~WebServer() {}

WebServer::WebServer(const WebServer &other)
{
	(void)other;
}

WebServer &WebServer::operator=(const WebServer &other)
{
	(void)other;
	return *this;
}

void WebServer::printServerBlocksInfo()
{
	for (std::vector<ServerBlock>::iterator it = this->_serverBlocks.begin(); it != this->_serverBlocks.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}
