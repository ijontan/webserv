#pragma once

#include "MethodIO.hpp"
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
	std::string query;
	std::map<std::string, std::string> envVariables;
	char **envV;
	void setEnv();
	void	setBody(std::string body);
	void setPath(const std::string path);

public:
	Cgi();
	Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers
	, std::string path, std::string body, std::string query);
	~Cgi();
	Cgi(const Cgi &src);
	Cgi &operator=(const Cgi &rhs);
	
	virtual int runCgi();
	std::string getPath() const;
	std::string getBody();
};
