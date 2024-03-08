
#include "webserv.h"
#include <iostream>
#include <vector>

ServerBlock::ServerBlock() : ABlock(), _locationBlocks()
{
}

ServerBlock::~ServerBlock()
{
}

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

void ServerBlock::addServerName(std::string serverName)
{
	this->_serverName.push_back(serverName);
}

void ServerBlock::addLocationBlock(std::string path,
								   LocationBlock locationBlock)
{
	this->_locationBlocks[path] = locationBlock;
}

std::ostream &operator<<(std::ostream &os, const ServerBlock &serverBlock)
{
	// print ports:
	os << "listen: ";
	print_vector(os, serverBlock.getPortsListeningOn());
	os << std::endl;

	// server names:
	os << "server_name: ";
	print_vector(os, serverBlock.getServerName());
	os << std::endl;

	// root directory:
	os << "root: " << serverBlock.getRootDirectory() << std::endl;

	// index:
	os << "index: ";
	print_vector(os, serverBlock.getIndex());
	os << std::endl;

	os << "client_max_body_size: " << serverBlock.getClientMaxBodySize()
	   << std::endl;

	// print error_pages:
	os << "error pages: " << std::endl;
	const std::map<int, std::string> &errorPages = serverBlock.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errorPages.begin();
		 it != errorPages.end(); it++)
	{
		os << (*it).first << " " << (*it).second << std::endl;
	}

	const std::pair<int, std::string> &redirection =
		serverBlock.getRedirection();
	os << "redirection: " << redirection.first << " " << redirection.second
	   << std::endl;

	return os;
}

void print_vector(std::ostream &os, const std::vector<std::string> &vector)
{
	for (std::vector<std::string>::const_iterator it = vector.begin();
		 it != vector.end(); it++)
	{
		os << *it << " ";
	}
}
