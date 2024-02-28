
#pragma once

#include <exception>
#include <string>
class CustomException : public std::exception
{
public:
	CustomException(std::string errorMsg);
	~CustomException() throw();

	const char *what() const throw();

private:
	std::string _errorMsg;
};

