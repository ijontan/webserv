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
		return;
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
			return;
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
	if (listen(sockfd, 10))
	{
		std::cerr << "listen error" << std::endl;
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

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void Socket::acceptLoop(void)
{
	char s[INET6_ADDRSTRLEN];

	while (1)
	{
		int newFd;
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof(their_addr);
		char buff[20];
		std::string str = "";

		newFd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
		inet_ntop(their_addr.ss_family,
				  get_in_addr((struct sockaddr *)&their_addr),
				  s, sizeof s);
		std::cout << HGREEN << "Server: got connection from: " << RESET << s << std::endl;
		while (recv(newFd, buff, 20, 0) > 0)
		{
			str += buff;
			memset(buff, 0, 20);
		}

		std::cout << BBLUE << "Message: " << RESET << std::endl
				  << str << std::endl
				  << BBLUE << "End" << RESET << std::endl
				  << "----------------------------------------" << std::endl;
		str = "Recieved message: " + str + "\nSending back: Hello, world!\n";
		send(newFd, str.c_str(), str.length(), 0);
		close(newFd);
	}
}
