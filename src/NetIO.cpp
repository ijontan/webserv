#include "NetIO.hpp"
#include <fstream>
#include <sstream>
#include <string>
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

std::string NetIO::getMessageToSend() const
{
	std::stringstream ss;
	std::vector<std::string> tokens = tokenize(getRaw());
	std::string path;
	if (tokens[1] == "/")
	{
		path = "www/index.html";
		ss << "HTTP/1.1 200 OK\r\n"
		   << "Content-Type: text/html\r\n\n";
	}
	else
	{
		path = "www" + tokens[1];
		ss << "HTTP/1.1 200 OK\r\n";
		if (tokens[1] == "/style.css")
			ss << "Content-Type: text/css\r\n\n";
		else
			ss << "Content-Type: text/js\r\n\n";
	}
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		return "cannot open!";
	}

	ss << file.rdbuf();
	file.close();
	return ss.str();
}

std::vector<std::string> NetIO::tokenize(std::string s) const
{
	std::istringstream iss(s);
	std::string token;
	std::vector<std::string> ret;

	while (std::getline(iss, token, ' '))
	{
		ret.push_back(token);
	}
	return ret;
}
