
#include "RequestException.hpp"
#include <string>

RequestException::RequestException(std::string errorMsg, int code) : _errorMsg(errorMsg), _code(code)
{
}

RequestException::~RequestException() throw()
{
}

const char *RequestException::what() const throw()
{
	return _errorMsg.c_str();
}

int RequestException::getCode() const throw()
{
	return _code;
}

