#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <map>
#include <vector>

class WebServer;

class Cgi
{
private:
	struct rInfo
	{
		std::vector<std::string> request;
		std::map<std::string, std::string> headers;
		std::string body;
	};
	std::string path;
	std::map<std::string, std::string> envVariables;
	char **envV;
	void setEnv(Cgi::rInfo &rqi);
	void tokenize(std::string s, Cgi::rInfo &rsi) const;

public:
	Cgi();
	Cgi(std::string path);
	~Cgi();
	Cgi(const Cgi &src);
	Cgi &operator=(const Cgi &rhs);
	
	virtual std::string runCgi(IOAdaptor &io, const std::string path);
	std::string getPath() const;
	void setPath(const std::string path);
};
