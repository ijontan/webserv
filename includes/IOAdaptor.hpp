#pragma once

#include <iostream>
#include <string>

class WebServer;

class IOAdaptor
{
private:
	std::string receivedRaw;

public:
	IOAdaptor(void);
	~IOAdaptor(void);
	IOAdaptor(const IOAdaptor &src);
	IOAdaptor &operator=(const IOAdaptor &rhs);
	virtual void receiveMessage(std::string raw);
	virtual std::string getMessageToSend(WebServer &ws, std::string port);
	std::string getRaw() const;
};

std::ostream &operator<<(std::ostream &os, const IOAdaptor &adaptor);
