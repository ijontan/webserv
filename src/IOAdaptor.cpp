#include "IOAdaptor.hpp"
#include "colors.h"
#include <sstream>

IOAdaptor::IOAdaptor(void) : receivedRaw("")
{
}

IOAdaptor::IOAdaptor(const IOAdaptor &src)
{
	*this = src;
}

IOAdaptor &IOAdaptor::operator=(const IOAdaptor &rhs)
{
	this->receivedRaw = rhs.receivedRaw;
	return *this;
}

IOAdaptor::~IOAdaptor(void)
{
}

void IOAdaptor::recieveMessage(std::string raw)
{
	receivedRaw = raw;
}

std::string IOAdaptor::getMessageToSend() const
{
	std::stringstream ss;
	ss << BGREEN << "Recieved message:\n"
	   << RESET << receivedRaw << BBLUE << "\nSending back: Hello, world!\n"
	   << RESET;
	return ss.str();
}

std::string IOAdaptor::getRaw() const
{
	return receivedRaw;
}

std::ostream &operator<<(std::ostream &os, const IOAdaptor &adaptor)
{
	os << BBLUE << "Message: " << RESET << std::endl
	   << adaptor.getRaw() << std::endl
	   << BBLUE << "End" << RESET << std::endl
	   << "----------------------------------------" << std::endl;
	return os;
}
