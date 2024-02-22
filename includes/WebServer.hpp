
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
	WebServer(const std::string &filePath, IOAdaptor &io);
	~WebServer();

	// utils
	void printServerBlocksInfo();
	void initSockets();
	void loop();
	void removePfd(int index);
	void addPfd(int fd);
	void addPfds(std::vector<int> fds);

private:
	WebServer(const WebServer &other);
	WebServer &operator=(const WebServer &other);
	void acceptConnection(int index, std::map<int, std::string> &buffMap);
	void handleIO(int index, std::map<int, std::string> &buffMap);

	std::vector<ServerBlock> _serverBlocks;
	std::vector<struct pollfd> _pfds;
	IOAdaptor &_io;
};

#endif
