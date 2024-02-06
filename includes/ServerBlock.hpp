
#include "webserv.h"
#include <vector>

#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

class LocationBlock;

class ServerBlock : public ABlock
{
public:
	ServerBlock();
	ServerBlock(const ServerBlock &other);
	ServerBlock &operator=(const ServerBlock &other);
	~ServerBlock();

	void addPortsListeningOn(std::string port);
	void setServerName(std::string serverName);

	void addLocationBlock(std::string path, LocationBlock locationBlock);

	std::vector<int> getSockfds() const;
	void initSockets();

private:
	void initSocket(std::string port);
	std::vector<int> sockfds;
	std::map<std::string, LocationBlock> _locationBlocks;
};

std::ostream &operator<<(std::ostream &os, const ServerBlock &serverBlock);

#endif
