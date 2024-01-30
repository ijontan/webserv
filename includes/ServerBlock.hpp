
#include "webserv.h"

#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

class LocationBlock;

class ServerBlock {
	public:
		ServerBlock();
		ServerBlock( const ServerBlock& other );
		ServerBlock& operator=( const ServerBlock& other );
		~ServerBlock();

	private:
		std::vector<int>				_portsListeningOn;
		std::string						_serverName;
		std::string						_rootDirectory;
		std::string						_index;
		// int								_limitClientBodySize;
		// data_type					cgi_scripts
		std::map<int, std::string> 		_errorPages;

		std::vector<LocationBlock>		_locationBlocks;
};


#endif
