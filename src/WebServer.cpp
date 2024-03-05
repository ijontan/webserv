
#include "IOAdaptor.hpp"
#include "colors.h"
#include "webserv.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

WebServer::WebServer(const std::string &filePath, IOAdaptor &io) : _io(io)
{
	Parser parser(filePath);

	parser.parseServerBlocks(this->_serverBlocks);
	std::cout << GREEN "Server blocks created" RESET << std::endl << std::endl;

	printServerBlocksInfo();
	initSockets();
}

WebServer::~WebServer()
{
	for (size_t i = 0; i < _pfds.size(); i++) {
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

void WebServer::initSockets()
{
	for (std::vector<ServerBlock>::iterator it = _serverBlocks.begin(); it < _serverBlocks.end(); it++)
	{
		(*it).initSockets();
		addPfds((*it).getSockfds());
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
			bool found = false;
			for (size_t j = 0; j < _serverBlocks.size(); j++)
				if (find(_serverBlocks[j].getSockfds(), _pfds[i].fd))
					found = true;

			if (found)
				acceptConnection(i, buffMap);
			else
				handleIO(i, buffMap);
		}
	}
}

void WebServer::acceptConnection(int index, std::map<int, std::string> &buffMap)
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
	addPfd(newFd);
}

void WebServer::handleIO(int index, std::map<int, std::string> &buffMap)
{
	char buff[256];
	buff[255] = 0;
	memset(buff, 0, sizeof(buff));
	int bytes = recv(_pfds[index].fd, buff, sizeof(buff) - 1, 0);
	// std::cout << WHITE << bytes << ": \n" << BLUE << buff << std::endl;
	buffMap[_pfds[index].fd] += buff;
	if (bytes < 255)
	{
		_io.receiveMessage(buffMap[_pfds[index].fd]);
		if (bytes >= 0)
		{
			std::cout << _io;
			std::string toSend = _io.getMessageToSend(*this);
			send(_pfds[index].fd, toSend.c_str(), toSend.length(), 0);
		}
		else
			std::cerr << "recv error" << std::endl;
		buffMap.erase(buffMap.find(_pfds[index].fd));
		close(_pfds[index].fd);
		_io.receiveMessage("");
		removePfd(index);
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
