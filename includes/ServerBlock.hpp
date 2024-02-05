
#include "webserv.h"

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

	void initSocket(void);
	int getSockfd() const;

private:
	int sockfd;
	std::map<std::string, LocationBlock> _locationBlocks;
};

std::ostream &operator<<(std::ostream &os, const ServerBlock &serverBlock);

#endif
