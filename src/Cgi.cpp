#include "Cgi.hpp"
#include "IOAdaptor.hpp"
#include "colors.h"
#include "utils.hpp"
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

Cgi::Cgi()
{
}

Cgi::Cgi(std::vector<std::string> request, std::map<std::string, std::string> headers, std::string path,
		 std::string body, std::string query)
	: request(request), header(headers), body(body), query(query)
{
	setPath(path);
}

Cgi::~Cgi()
{
	if (!envV)
		return;
	for (int i = 0; i < (int)(this->envVariables.size() + 1); i++)
		free(this->envV[i]);
	free(this->envV);
}

Cgi::Cgi(const Cgi &src)
{
	*this = src;
}

// set env variables for execvpe
void Cgi::setEnv()
{
	std::stringstream convert;
	convert << this->query.size();

	this->envVariables["PATH_INFO"] = getPath();
	this->envVariables["REQUEST_METHOD"] = this->request[0];

	if (this->envVariables["REQUEST_METHOD"] == "GET")
		this->envVariables["QUERY_STRING"] = this->query;
	
	if (this->envVariables["REQUEST_METHOD"] == "POST")
	{
		this->envVariables["CONTENT_LENGTH"] = convert.str();
		this->envVariables["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
		// this->envVariables["QUERY_STRING"] = this->query;
	}

	// std::cout << "	content length: " << this->envVariables["CONTENT_LENGTH"] << std::endl;
	// std::cout << "	content type: " << this->envVariables["CONTENT_TYPE"] << std::endl;
	// std::cout << "	query: " << this->envVariables["QUERY_STRING"] << std::endl;
	
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
		// if (close(fd[0]) == -1)
		// 	return (500);
		// std::cout << "test1" << std::endl;
		dup2(fd[1], STDOUT_FILENO);
		// std::cout << "test2" << std::endl;
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		// dup2(fd[1], STDERR_FILENO);
		execve(this->path.c_str(), av, this->envV);
		exit(-1);
	}
	else
	{
		if (this->request[0] == "POST")
		{
			if (write(fd[1], this->query.c_str() ,this->query.size()) == -1)
				return (500);
		}
		waitpid(pid, &status, 0);
		close(fd[1]);
		int read_bytes;

		if (!WIFEXITED(status))
			return (500);
		read_bytes = 1;
		while (read_bytes > 0)
		{
			memset(buf, 0, 2048);
			read_bytes = read(fd[0], buf, 2048);
			outputString += buf;
		}
		close(fd[0]);
	}
	// if (close(fd[0]) == -1)
	// 	return (500);
	setBody(outputString);
	// close(fd[0]);
	// close(fd[1]);
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
	this->path = path;
}

void Cgi::setBody(std::string body)
{
	this->body = body;
}
