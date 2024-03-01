#pragma once

#include <iostream>
#include <string>

class WebServer;

class Cgi
{
private:
	std::string receivedRaw;

public:
	Cgi();
	~Cgi();
	Cgi(const Cgi &src);
	Cgi &operator=(const Cgi &rhs);
	std::string getRaw() const;
	virtual void recieveMessage(std::string raw);
	virtual std::string getMessageToSend(WebServer &ws);
};
