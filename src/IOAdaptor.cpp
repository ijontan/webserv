#include "IOAdaptor.hpp"
#include "colors.h"

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
    return "Recieved message:\n" + receivedRaw + "\nSending back: Hello, world!\n";
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