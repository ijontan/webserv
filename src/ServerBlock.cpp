
#include "webserv.h"

ServerBlock::ServerBlock() : ABlock(),
							 _locationBlocks()
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

void ServerBlock::setServerName(std::string serverName)
{
	this->_serverName = serverName;
}

void ServerBlock::addLocationBlock(std::string path, LocationBlock locationBlock)
{
	this->_locationBlocks[path] = locationBlock;
}

int ServerBlock::getSockfd() const
{
	return sockfd;
}

void ServerBlock::initSocket()
{
	struct addrinfo hints, *servInfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	 // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, getPortsListeningOn()[0].c_str(), &hints, &servInfo) != 0)
	{
		std::cerr << "getaddrinfo error" << std::endl;
		throw "getaddrinfo error";
	}
	for (p = servInfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			std::cerr << "socket error" << std::endl;
			continue;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockfd, sizeof(int)) == -1)
		{
			std::cerr << "Error setting socket options" << std::endl;
			close(sockfd); // Don't forget to close the socket in case of an error
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
	std::cout << HWHITE << "Server: waiting for connections..." << RESET << std::endl
			  << std::endl;
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
