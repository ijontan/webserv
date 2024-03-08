#include "Cgi.hpp"
#include "colors.h"
#include "IOAdaptor.hpp"
#include "utils.hpp"
#include <sstream>
#include <stdlib.h>

Cgi::Cgi()
{}

Cgi::Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers
	, std::string body) : request(request), header(headers), body(body)
{
}

Cgi::~Cgi(){}

Cgi::Cgi(const Cgi &src)
{
	*this = src; 
}

// set env variables for execve
void Cgi::setEnv()
{
	this->envVariables["PATH_INFO"] = getPath();
	this->envVariables["REQUEST_METHOD"] = this->request[0];
	this->envV = (char **)calloc(sizeof(char *), this->envVariables.size() + 1);
	std::map<std::string, std::string>::const_iterator it = this->envVariables.begin();
	for (int i = 0; it != this->envVariables.end(); i++, it++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->envV[i] = strdup(tmp.c_str());
	}
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	(void)rhs;
	return *this;
}

std::string Cgi::runCgi()
{
	pid_t pid;
	int fd[2];
	int status;
	std::string output;
	char buf[2048] = {0};

	setPath(this->request[1]);
	setEnv();
	// dir = file directory
	std::string dir = getPath().substr(0, getPath().find_first_of("/", 2) + 1);
	this->path = getPath();
	char *av[3] = {(char *)this->path.c_str(), (char *)dir.c_str(), NULL};

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		int i = execve(this->path.c_str(), av, this->envV);
		exit(i);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		int read_bytes;

		if (WEXITSTATUS(status) && WIFEXITED(status) != 0)
			return ("\0");
		read_bytes = 1;
		while (read_bytes > 0)
		{
			memset(buf, 0, 2048);
			read_bytes = read(fd[0], buf, 2048);
			output += buf;
		}
	}
	close(fd[1]);
	close(fd[0]);
	return (output);
}

std::string Cgi::getPath() const
{
	return this->path == "/" ? "./cgi-bin/test.py" : "www" + this->path;
	return this->path;
}

void Cgi::setPath(const std::string path)
{
	this->path = path;
}
