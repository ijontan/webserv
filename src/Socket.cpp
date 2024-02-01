#include "Socket.hpp"
#include <arpa/inet.h>
#include "colors.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket(void) : sockfd(-1), port("8080")
{
}

Socket::Socket(const std::string &port) : sockfd(-1), port(port)
{
}

void Socket::initSocket()
{
	struct addrinfo hints, *servInfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	 // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port.c_str(), &hints, &servInfo) != 0)
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

Socket::Socket(const Socket &src)
{
	*this = src;
}

Socket &Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
	{
		this->sockfd = rhs.sockfd;
		this->port = rhs.port;
	}
	return *this;
}

Socket::~Socket(void)
{
	close(sockfd);
}

int Socket::getSockfd() const
{
	return sockfd;
}

std::ostream &operator<<(std::ostream &os, const Socket &socket)
{
	os << socket.getSockfd() << std::endl;
	return os;
}