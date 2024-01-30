
#include "webserv.h"

CustomException::CustomException( std::string errorMsg ): _errorMsg(errorMsg) {}

CustomException::~CustomException() _NOEXCEPT {}

const char *CustomException::what() const throw() {
	return _errorMsg.c_str();
}
