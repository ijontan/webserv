#include "NetIO.hpp"
#include "webserv.h"
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

NetIO::NetIO(void) : IOAdaptor()
{
}

NetIO::NetIO(const NetIO &src) : IOAdaptor()
{
	*this = src;
}

NetIO &NetIO::operator=(const NetIO &rhs)
{
	(void)rhs;
	return *this;
}

NetIO::~NetIO(void)
{
}

std::string NetIO::getMessageToSend()
{
	std::stringstream ss;
	std::string path;

	NetIO::requestInfo ri;

	tokenize(getRaw(), ri);

	if (ri.request[1] == "/")
	{
		path = "www/index.html";
		ss << "HTTP/1.1 200 OK\r\n"
		   << "Content-Type: text/html\r\n";
	}
	else
	{
		path = "www" + ri.request[1];
		ss << "HTTP/1.1 200 OK\r\n";
		ss << "Content-Type: text/" << utils::split( ri.request[1], '.').back() << "\r\n";
	}
	ss << "\n";
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		return "cannot open!";
	}

	ss << file.rdbuf();
	file.close();
	return ss.str();
}

void NetIO::tokenize(std::string s, NetIO::requestInfo &ri) const
{
	std::pair<std::string, std::string> headerBody = utils::splitPair(s, "\r\n\n");
	std::vector<std::string> requestHeader = utils::split(headerBody.first, "\r\n");

	ri.request = utils::split(requestHeader[0], ' ');
	for (size_t i = 1; i < requestHeader.size(); i++)
		ri.headers.insert(utils::splitPair(requestHeader[i], ": "));
	ri.body = headerBody.second;
}
