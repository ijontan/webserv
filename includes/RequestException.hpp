
#pragma once

#include <exception>
#include <map>
#include <string>
class RequestException : public std::exception
{
public:
	RequestException(std::string errorMsg, int code);
	~RequestException() throw();

	const char *what() const throw();
	int getCode() const throw();

private:
	std::string _errorMsg;
	int _code;
};
