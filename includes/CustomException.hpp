
#include "webserv.h"

#ifndef CUSTOMEXCEPTION_HPP
#define CUSTOMEXCEPTION_HPP

class CustomException: public std::exception {
	public:
		CustomException( std::string errorMsg );
		virtual ~CustomException() _NOEXCEPT;

		const char *what() const throw();

	private:
		std::string _errorMsg;
};


#endif

