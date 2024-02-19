
#include "webserv.h"
#include <vector>

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

class Parser;
class ServerBlock;
class IOAdaptor;

class WebServer
{
public:
	WebServer(const std::string &filePath);
	~WebServer();

	// utils
	void printServerBlocksInfo();
	void initSockets();
	void loop(IOAdaptor io);
	void removePfd(int index);
	void addPfd(int fd);
	void addPfds(std::vector<int> fds);

private:
	WebServer(const WebServer &other);
	WebServer &operator=(const WebServer &other);

	std::vector<ServerBlock> _serverBlocks;
	std::vector<struct pollfd> pfds;
};

#endif
