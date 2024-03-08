
#pragma once

#include <vector>
#include <string>
#include <map>
#include "Parser.hpp"
#include "ServerBlock.hpp"
#include "IOAdaptor.hpp"

class Parser;
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
	void acceptConnection(int index, std::map<int, std::string> &buffMap, std::string port);
	void handleIO(int index, std::map<int, std::string> &buffMap);

	std::vector<ServerBlock> _serverBlocks;
	std::vector<struct pollfd> _pfds;
	std::map<int, std::string> _socketPortmap;
	std::map<int, std::string> _connectionsPortMap;
	IOAdaptor &_io;
};

