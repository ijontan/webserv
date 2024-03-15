#include "Cgi.hpp"
#include "colors.h"
#include "IOAdaptor.hpp"
#include "utils.hpp"
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <sys/wait.h>

Cgi::Cgi()
{}

Cgi::Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers
	, std::string path, std::string body, std::string query) : request(request), header(headers), body(body), query(query)
{
	setPath(path);
}

Cgi::~Cgi()
{
	for (int i = 0; i < (int)(this->envVariables.size() + 1); i++)
		free(this->envV[i]);
	delete[] this->envV;
}

Cgi::Cgi(const Cgi &src)
{
	*this = src; 
}

// set env variables for execve
void Cgi::setEnv()
{
	this->envVariables["PATH_INFO"] = getPath();
	this->envVariables["REQUEST_METHOD"] = this->request[0];
	this->envVariables["QUERY_STRING"] = this->query;
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

int Cgi::runCgi()
{
	pid_t pid;
	int fd[2];
	int status;
	std::string outputString;
	char buf[2048] = {0};

	setEnv();
	// dir = file directory
	std::string dir = getPath().substr(0, getPath().find_first_of("/", 2) + 1);
	this->path = getPath();

	char *av[3] = {(char *)this->path.c_str(), (char *)dir.c_str(), NULL};

	if (pipe(fd) == -1)
		return (500);
	pid = fork();
	if (pid == 0)
	{
		if (close(fd[0]) == -1 )
			return (500);
		dup2(fd[1], STDOUT_FILENO);
		std::cout << this->envV[1] << std::cout;
		int i = execve(this->path.c_str(), av, this->envV);
		exit(i);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (close(fd[1]) == -1)
			return (500);
		dup2(fd[0], STDIN_FILENO);
		int read_bytes;

		if (WEXITSTATUS(status) && WIFEXITED(status) != 0)
			return (500);
		read_bytes = 1;
		while (read_bytes > 0)
		{
			memset(buf, 0, 2048);
			read_bytes = read(fd[0], buf, 2048);
			outputString += buf;
		}
	}
	// if (close(output[0]) == -1 || close(output[1]) == -1 || close(input[0]) == -1 || close(input[1]) == -1)
	// 	return (500);
	std::cout << outputString << std::endl;
	setBody(outputString);
	return (200);
}

std::string Cgi::getPath() const
{
	return (this->path);
}

std::string Cgi::getBody()
{
	return this->body;
}

void Cgi::setPath(const std::string path)
{
	std::cout << "PATH: " << path << std::endl;
	this->path = path;
}

void	Cgi::setBody(std::string body)
{
	std::cout << "test" << std::endl;
	std::cout << body << std::endl;
	this->body = body;
}