#pragma once

#include "IOAdaptor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <map>
#include <vector>

class Cgi
{
private:
	// struct rInfo
	// {
	// 	std::vector<std::string> request;
	// 	std::map<std::string, std::string> headers;
	// 	std::string body;
	// };
	std::vector<std::string> request;
	std::map<std::string, std::string> header;
	std::string body;
	std::string path;
	std::map<std::string, std::string> envVariables;
	char **envV;
	void setEnv();

public:
	Cgi();
	Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers
	, std::string body);
	~Cgi();
	Cgi(const Cgi &src);
	Cgi &operator=(const Cgi &rhs);
	
	virtual std::string runCgi();
	std::string getPath() const;
	void setPath(const std::string path);
};
