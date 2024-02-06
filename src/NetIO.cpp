#include "NetIO.hpp"

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
    if (getRaw() == "wanna fight?")
        return "fuck you bitch";
    else
        return "ok";
}