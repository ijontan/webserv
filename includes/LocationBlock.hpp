
#include "webserv.h"

#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

class LocationBlock {
	public:
		LocationBlock();
		LocationBlock( const LocationBlock& other );
		LocationBlock& operator=( const LocationBlock& other );
		~LocationBlock();

		// setters
		void	setAutoindexStatus( bool status );
		void	addAllowedMethods( std::string path );
		void	setRedirection( int statusCode, std::string path );
		void	setCgiPassPath( std::string path );

	private:
		bool						_autoindexStatus;
		std::vector<std::string>	_allowedMethods;
		std::pair<int, std::string>	_redirection;
		std::string					_cgiPass;
};


#endif
