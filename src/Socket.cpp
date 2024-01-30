#include "Socket.hpp"
#include <arpa/inet.h>
#include "colors.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket(void) : sockfd(-1), port("8080"), pfdCount(0)
{
}

Socket::Socket(const std::string &port) : sockfd(-1), port(port), pfdCount(0)
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
	addPfd(sockfd);
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

void Socket::acceptLoop(IOAdaptor &nio)
{
	char s[INET6_ADDRSTRLEN];
	int newFd;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	char buff[256];
	std::vector<std::string> str = std::vector<std::string>();

	str.push_back("");
	for (;;)
	{
		int pollCount = poll(fds, pfdCount, -1);
		if (pollCount == -1)
		{
			std::cerr << "poll error" << std::endl;
			return;
		}

		for (int i = 0; i < pfdCount; i++)
		{
			if (!(fds[i].revents & POLLIN))
				continue;
			if (fds[i].fd == sockfd)
			{
				addr_size = sizeof(their_addr);
				newFd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
				if (newFd == -1)
				{
					std::cerr << "accept error" << std::endl;
					continue;
				}
				inet_ntop(their_addr.ss_family,
						  get_in_addr((struct sockaddr *)&their_addr),
						  s, sizeof(s));
				std::cout << HGREEN << "Server: got connection from: " << RESET << s << std::endl;
				addPfd(newFd);
				str.push_back("");
			}
			else
			{
				int bytes = recv(fds[i].fd, buff, sizeof(buff), 0);
				if (bytes <= 0)
				{
					nio.recieveMessage(str[i]);
					if (bytes == 0)
					{
						std::cout << nio;
						std::string toSend = nio.getMessageToSend();
						send(newFd, toSend.c_str(), toSend.length(), 0);
					}
					else
					{
						std::cerr << "recv error" << std::endl;
					}
					str.erase(str.begin() + i);
					close(fds[i].fd);
					removePfd(i);
					nio.recieveMessage("");
				}
				else
				{
					str[i] += buff;
					memset(buff, 0, sizeof(buff));
				}
			}
		}
	}
}

void Socket::addPfd(int fd)
{
	fds[pfdCount].fd = fd;
	fds[pfdCount].events = POLLIN;
	pfdCount++;
}

void Socket::removePfd(int index)
{
	fds[index] = fds[pfdCount - 1];
	pfdCount--;
}