
#include "IOAdaptor.hpp"
#include "colors.h"
#include "webserv.h"
#include <cstddef>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

WebServer::WebServer(const std::string &filePath, IOAdaptor &io) : _io(io)
{
	try
	{
		Parser parser(filePath);

		parser.parseServerBlocks(this->_serverBlocks);
		std::cout << GREEN "Server blocks created" RESET << std::endl << std::endl;

		printServerBlocksInfo();
		initSockets();
	}
	catch (const std::exception &e)
	{
		std::cout << BRED << e.what() << RESET << std::endl;
	}
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < _pfds.size(); i++)
	{
		close(_pfds[i].fd);
	}
}

WebServer::WebServer(const WebServer &other) : _io(other._io)
{
	(void)other;
}

WebServer &WebServer::operator=(const WebServer &other)
{
	(void)other;
	return *this;
}

void WebServer::printServerBlocksInfo()
{
	for (std::vector<ServerBlock>::iterator it = this->_serverBlocks.begin(); it != this->_serverBlocks.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}

int initSocket(std::string port)
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
		throw "error";
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
	std::cout << HWHITE << "Server: waiting for connections..." << RESET << std::endl << std::endl;
	return sockfd;
}

void WebServer::initSockets()
{
	for (std::vector<ServerBlock>::iterator it = _serverBlocks.begin(); it < _serverBlocks.end(); it++)
	{
		std::vector<std::string> ports = it->getPortsListeningOn();
		for (size_t i = 0; i < ports.size(); i++)
		{
			std::stringstream ss(ports[i]);
			int port;
			ss >> port;
			std::map<int, std::string>::iterator it;
			for ( it = _socketPortmap.begin(); it!= _socketPortmap.end(); it++) 
				if  (it->second == ports[i]) break;
			
			if (it== _socketPortmap.end())
			{
				try
				{
					int fd = initSocket(ports[i]);
					addPfd(fd);
					_socketPortmap.insert(std::make_pair(fd, ports[i]));
					std::cout << "fd: " << fd << std::endl;
				}
				catch (char const *e)
				{
					std::cerr << e << std::endl;
				}
			}
		}
	}
}

static void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

template <typename T>
static bool find(std::vector<T> arr, T value)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] == value)
		{
			return true;
		}
	}
	return false;
}

void WebServer::loop()
{
	std::map<int, std::string> buffMap;

	for (size_t j = 0; j < _serverBlocks.size(); j++)
		std::cout << _serverBlocks[j];

	for (;;)
	{
		int pollCount = poll(&_pfds[0], _pfds.size(), -1);
		if (pollCount == -1)
		{
			std::cerr << "poll error" << std::endl;
			return;
		}

		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (!(_pfds[i].revents & POLLIN))
				continue;

			// find if socket exist
			std::map<int, std::string>::iterator port = _socketPortmap.find(_pfds[i].fd);

			if (port != _socketPortmap.end())
				acceptConnection(i, buffMap, port->second);
			else
				handleIO(_pfds[i].fd, buffMap);
		}
	}
}

void WebServer::acceptConnection(int index, std::map<int, std::string> &buffMap, std::string port)
{
	struct sockaddr_storage theiraddr;
	socklen_t addrSize = sizeof(theiraddr);
	char s[INET6_ADDRSTRLEN];
	int newFd = accept(_pfds[index].fd, (struct sockaddr *)&theiraddr, &addrSize);
	if (newFd == -1)
	{
		std::cerr << "accept error" << std::endl;
		return;
	}
	inet_ntop(theiraddr.ss_family, get_in_addr((struct sockaddr *)&theiraddr), s, sizeof(s));
	std::cout << HGREEN << "Server: got connection from: " << RESET << s << std::endl;
	buffMap.insert(std::pair<int, std::string>(newFd, ""));
	_connectionsPortMap.insert(std::make_pair(newFd, port));
	addPfd(newFd);
}

void WebServer::handleIO(int fd, std::map<int, std::string> &buffMap)
{
	char buff[256];

	buff[255] = 0;
	memset(buff, 0, sizeof(buff));
	int bytes = recv(fd, buff, sizeof(buff) - 1, 0);
	buffMap[fd] += buff;
	if (bytes < 255)
	{
		_io.receiveMessage(buffMap[fd]);
		if (bytes >= 0)
		{
			std::cout << _io;
			std::string toSend = _io.getMessageToSend(*this, _connectionsPortMap[fd]);
			send(fd, toSend.c_str(), toSend.length(), 0);
		}
		else
			std::cerr << "recv error" << std::endl;
		buffMap.erase(buffMap.find(fd));
		_connectionsPortMap.erase(_connectionsPortMap.find(fd));
		close(_pfds[fd].fd);
		_io.receiveMessage("");
		removePfd(fd);
	}
}

void WebServer::addPfd(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	_pfds.push_back(pfd);
}

void WebServer::addPfds(std::vector<int> fds)
{
	for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		addPfd(*it);
		std::cout << "fd: " << *it << std::endl;
	}
}

void WebServer::removePfd(int index)
{
	_pfds.erase(_pfds.begin() + index);
}
