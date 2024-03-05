#include "Cgi.hpp"
#include "colors.h"
#include "IOAdaptor.hpp"
#include "utils.hpp"
#include <sstream>
#include <stdlib.h>

Cgi::Cgi()
{}

Cgi::Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers
	, std::string body)
{
	this->request = request;
	this->header = headers;
	this->body = body;
}

Cgi::~Cgi(){}

Cgi::Cgi(const Cgi &src)
{
	*this = src; 
}

void Cgi::setEnv()
{
	this->envVariables["SCRIPT_NAME"] = getPath();
	this->envVariables["REQUEST_METHOD"] = this->request[0];
	this->envV = (char **)calloc(sizeof(char *), this->envVariables.size() + 1);
	std::map<std::string, std::string>::const_iterator it = this->envVariables.begin();
	for (int i = 0; it != this->envVariables.end(); i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->envV[i] = strdup(tmp.c_str());
		i++;
	}
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	return *this;
}

std::string Cgi::runCgi()
{
	pid_t pid;
	int input[2];
	int output[2];

	setPath(this->request[1]);
	std::string dir = getPath().substr(0, getPath().find_first_of("/", 2) + 1);

	char *av[3] = {(char *)this->path.c_str(), (char *)dir.c_str, NULL};

	if (pipe(input) == -1 || pipe(output) == -1)
		return ("fail\n");
	pid = fork();
	if (pid == 0)
	{
		dup2(input[0], STDIN_FILENO);
		dup2(output[1], STDOUT_FILENO);
		close(input[0]);
		close(input[1]);
		close(output[0]);
		close(output[1]);
		int i = execve(this->path.c_str(), av, this->envV);
		if (i < 0)
			exit(i);
	}
}

std::string Cgi::getPath() const
{
	return this->path;
}

void Cgi::setPath(const std::string path)
{
	this->path = path;
}
