
#include "webserv.h"

ServerBlock::ServerBlock() : ABlock(),
							 _locationBlocks()
{
}

ServerBlock::~ServerBlock() {}

ServerBlock::ServerBlock(const ServerBlock &other) : ABlock()
{
	*this = other;
}

ServerBlock &ServerBlock::operator=(const ServerBlock &other)
{
	if (this != &other)
	{
		// copy assignment of base class ABlock
		ABlock::operator=(other);

		this->_locationBlocks = other._locationBlocks;
	}
	return *this;
}

void ServerBlock::addPortsListeningOn(std::string port)
{
	this->_portsListeningOn.push_back(port);
}

void ServerBlock::setServerName(std::string serverName)
{
	this->_serverName = serverName;
}

void ServerBlock::addLocationBlock(std::string path, LocationBlock locationBlock)
{
	this->_locationBlocks[path] = locationBlock;
}

std::ostream &operator<<(std::ostream &os, const ServerBlock &serverBlock)
{
	// print ports:
	os << "listen: ";
	const std::vector<std::string> &ports = serverBlock.getPortsListeningOn();
	for (std::vector<std::string>::const_iterator it = ports.begin(); it != ports.end(); it++)
	{
		os << *it << " ";
	}
	os << std::endl;

	os << "server_name: " << serverBlock.getServerName() << std::endl;
	os << "root: " << serverBlock.getRootDirectory() << std::endl;
	os << "index: " << serverBlock.getIndex() << std::endl;
	os << "client_max_body_size: " << serverBlock.getClientMaxBodySize() << std::endl;

	// print error_pages:
	os << "error pages: " << std::endl;
	const std::map<int, std::string> &errorPages = serverBlock.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); it++)
	{
		os << (*it).first << " " << (*it).second << std::endl;
	}

	const std::pair<int, std::string> &redirection = serverBlock.getRedirection();
	os << "redirection: " << redirection.first << " " << redirection.second << std::endl;

	return os;
}
