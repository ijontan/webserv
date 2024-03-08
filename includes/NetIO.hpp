#pragma once

#include "IOAdaptor.hpp"
#include <map>
#include <string>
#include <vector>

class NetIO : public IOAdaptor
{
private:
	struct requestInfo
	{
		std::vector<std::string> request;
		std::map<std::string, std::string> headers;
		std::string body;
	};
	void tokenize(std::string s, NetIO::requestInfo &ri) const;

public:
	NetIO(void);
	~NetIO(void);
	NetIO(const NetIO &src);
	NetIO &operator=(const NetIO &rhs);
	std::string getMessageToSend(WebServer &ws, std::string port);
};
