
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

std::vector<int> ServerBlock::getSockfds() const
{
	return sockfds;
}

void ServerBlock::initSocket(std::string port)
{
	struct addrinfo hints, *servInfo, *p;
	int sockfd;

	memset(&hints, 0, sizeof(hints));
	servInfo = 0;
	hints.ai_family = AF_UNSPEC;	 // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;
	std::cout << "port: " << port << std::endl;
	if (getaddrinfo(NULL, port.c_str(), &hints, &servInfo) != 0)
	{
		std::cerr << "getaddrinfo error" << std::endl;
		return;
	}
	for (p = servInfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
			-1)
		{
			std::cerr << "socket error" << std::endl;
			continue;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockfd,
					   sizeof(int)) == -1)
		{
			std::cerr << "Error setting socket options" << std::endl;
			close(
				sockfd); // Don't forget to close the socket in case of an error
			throw "Error setting socket options";
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			std::cerr << "bind error" << std::endl;
			continue;
		}
		break;
	}
	freeaddrinfo(servInfo); // free the linked list
	if (p == NULL)
	{
		std::cerr << "failed to bind" << std::endl;
		throw "fail to bind";
	}
	if (listen(sockfd, 10))
	{
		std::cerr << "listen error" << std::endl;
		throw "fail to listen";
	}
	std::cout << HWHITE << "Server: waiting for connections..." << RESET
			  << std::endl
			  << std::endl;
	sockfds.push_back(sockfd);
}

void ServerBlock::initSockets()
{
	std::vector<std::string> ports = getPortsListeningOn();
	for (std::vector<std::string>::iterator it = ports.begin();
		 it < ports.end(); it++)
	{
		initSocket(*it);
	}
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
