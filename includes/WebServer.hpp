
#pragma once

#include "IOAdaptor.hpp"
#include "MethodIO.hpp"
#include "ServerBlock.hpp"
#include <map>
#include <string>
#include <vector>

class Parser;
class IOAdaptor;
class MethodIO;

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
	std::vector<ServerBlock> &getServers();

private:
	WebServer(const WebServer &other);
	WebServer &operator=(const WebServer &other);
	void acceptConnection(int index, std::map<int, std::string> &buffMap, std::string port);
	void handleIO(int index, std::map<int, std::string> &buffMap);
	MethodIO::rInfo parseHeader(std::string str);

	std::vector<ServerBlock> _serverBlocks;
	std::vector<struct pollfd> _pfds;
	std::map<int, std::string> _socketPortmap;
	std::map<int, std::string> _connectionsPortMap;
	IOAdaptor &_io;
};
