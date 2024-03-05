#include "Cgi.hpp"
#include "colors.h"
#include "IOAdaptor.hpp"
#include "utils.hpp"
#include <sstream>
#include <stdlib>

Cgi::Cgi(): path("")
{}

Cgi::Cgi(std::string path) : path(path)
{}

Cgi::~Cgi(){}

Cgi::Cgi(const Cgi &src)
{
	*this = src; 
}

void Cgi::setEnv(Cgi::rInfo &rqi)
{
	this->envVariables["SCRIPT_NAME"] = rqi.request[1];
	this->envVariables["REQUEST_METHOD"] = rqi.request[0];
	this->envV = (char **)calloc(sizeof(char *), this->envVariables.size() + 1);
	std::map<std::string, std::string>::const_iterator it = this->envVariables.begin();
	for (int i = 0; it != this->envVariables.end(); it++; i++)
	{
		std::string tmp = it->first + "=" it->second;
		this->envV[i] = strdup(tmp.c_str());
	}
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	return *this;
}

std::string Cgi::runCgi(IOAdaptor &io, const std::string path)
{
	pid_t pid;
	int input[2];
	int output[2];
	Cgi::rInfo rqi;

	tokenize(io.getRaw(), rqi);
	// setPath(path);
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
		int i = execve(this->path.c_str(), av, this->envV)
		if (i < 0)
			exit(i);
	}
}

// std::string Cgi::getPath() const
// {
// 	return this->path;
// }

// void Cgi::setPath(const std::string path)
// {
// 	this->path = path;
// }

void Cgi::tokenize(std::string s, Cgi::rInfo &rsi) const
{
	std::pair<std::string, std::string> headerBody = utils::splitPair(s, "\r\n\n");
	std::vector<std::string> requestHeader = utils::split(headerBody.first, "\r\n");

	rsi.request = utils::split(requestHeader[0], ' ');
	for (size_t i = 1; i < requestHeader.size(); i++)
		rsi.headers.insert(utils::splitPair(requestHeader[i], ": "));
	rsi.body = headerBody.second;
}