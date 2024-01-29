#pragma once

#include <string>
#include <iostream>

class IOAdaptor
{
private:
    std::string receivedRaw;

public:
    IOAdaptor(void);
    ~IOAdaptor(void);
    IOAdaptor(const IOAdaptor &src);
    IOAdaptor &operator=(const IOAdaptor &rhs);
    virtual void recieveMessage(std::string raw);
    virtual std::string getMessageToSend();
    std::string getRaw() const;
};

std::ostream &operator<<(std::ostream &os, const IOAdaptor &adaptor);