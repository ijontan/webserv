#include "Cgi.hpp"
#include "colors.h"
#include <sstream>

Cgi::Cgi(){}

Cgi::~Cgi(){}

Cgi::Cgi(const Cgi &src)
{
	*this = src; 
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	return *this;
}

std::string Cgi::getRaw() const
{
	return receivedRaw;
}

void Cgi::recieveMessage(std::string raw)
{
	receivedRaw = raw;
}

std::string Cgi::getMessageToSend(WebServer &ws)
{
	(void)ws;
	std::stringstream ss;
	ss << "Recieved message:\n"
	   << receivedRaw << "\nSending back: Hello, world!\n";
	return ss.str();
}