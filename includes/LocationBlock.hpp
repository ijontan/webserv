
#include "webserv.h"

#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

class LocationBlock {
	public:
		LocationBlock();
		LocationBlock( const LocationBlock& other );
		LocationBlock& operator=( const LocationBlock& other );
		~LocationBlock();

	private:

		// bool						_autoindex;
		std::vector<std::string>	_allowedMethods;
		std::pair<int, std::string>	_redirection;
		std::string					_cgiPass;
};


#endif
