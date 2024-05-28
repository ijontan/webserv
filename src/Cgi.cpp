#include "Cgi.hpp"
#include "IOAdaptor.hpp"
#include "colors.h"
#include "utils.hpp"
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "RequestException.hpp"

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

static std::string replace(std::string str, char from, char to)
{
    int l = str.length();
 
    for (int i = 0; i < l; i++) 
    {
        if (str[i] == from)
            str[i] = to;
		str[i] = toupper(str[i]);
    }
    return str;
}

// set env variables for execvpe
void Cgi::setEnv()
{
	// std::stringstream convert;
	// convert << this->query.size();

	this->envVariables["PATH_INFO"] = getPath();
	this->envVariables["REQUEST_METHOD"] = this->request[0];

	if (this->envVariables["REQUEST_METHOD"] == "GET")
	{
		this->envVariables["QUERY_STRING"] = this->query;
		this->envVariables["HTTP_COOKIE"] = this->header["Cookie"];
	}
	
	if (this->envVariables["REQUEST_METHOD"] == "POST")
		this->envVariables["HTTP_COOKIE"] = this->header["Cookie"];

	std::map<std::string, std::string>::const_iterator it;
	for (it = this->header.begin(); it != this->header.end(); it++)
	{
		this->envVariables[replace(it->first, '-', '_')] = it->second;
	}
	
	this->envV = (char **)calloc(this->envVariables.size() + 1, sizeof(char *));
	it = this->envVariables.begin(); 
	for (int i = 0; it != this->envVariables.end(); i++, it++)
	{
		std::string tmp = replace(it->first, '-', '_') + "=" + it->second;
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
	// int fd[2];
	int input[2];
	int	output[2];
	int status;
	std::string outputString;

	setEnv();
	// dir = file directory
	std::string dir = getPath().substr(0, getPath().find_first_of("/", 2) + 1);
	this->path = getPath();
	char *av[3] = {(char *)this->path.c_str(), (char *)dir.c_str(), NULL};

	// if (pipe(fd) == -1)
	if (access(this->path.c_str(), X_OK))
		throw RequestException("File read forbidden", 403);
	if (pipe(input) == -1 || pipe(output) == -1)
		return (500);
	pid = fork();
	if (pid == 0)
	{
		close(output[0]);
		close(input[1]);
		dup2(output[1], STDOUT_FILENO);
		close(output[1]);
		dup2(input[0], STDIN_FILENO);
		close(input[0]);
		execve(this->path.c_str(), av, this->envV);
		exit(127);
	}
	else
	{
		close(input[0]);
		if (write(input[1], this->body.c_str() ,this->body.size()) == -1)
			return (500);
		close(input[1]);
		close(output[1]);

		int limit = 0;
		while (limit < TIMEOUT)
		{
			sleep(1);
			int lim = waitpid(pid, &status, WNOHANG);
			if (lim < 0)
				throw RequestException("Internal Server Error",  500);
			if (WIFEXITED(status))
				break;
			limit++;
		}
		if (limit == TIMEOUT)
		{
			kill(pid, 9);
			throw RequestException("Request Timeout",  408);
		}
		int read_bytes;

		read_bytes = 1;
		char buf[1024];
		while (read_bytes > 0)
		{
			memset(buf, 0, 1024);
			read_bytes = read(output[0], buf, 1024);
			outputString.append(buf, read_bytes);
		}
		close(output[0]);
	}
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
	this->path = path;
}

void Cgi::setBody(std::string body)
{
	this->body = body;
}
