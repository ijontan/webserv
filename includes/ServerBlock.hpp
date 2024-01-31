
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

		// setters
		void	addPortsListeningOn( int port );
		void	setServerName( std::string serverName );
		void	setRootDirectory( std::string rootDirectory );
		void	setIndex( std::string index );
		void	setLimitClientBodySize( int limitClientBodySize );
		// data_type	cgi_scripts
		void 	addErrorPage( int statusCode, std::string uri );
		void 	addLocationBlock( std::string path, LocationBlock locationBlock );

	private:
		std::vector<int>								_portsListeningOn;
		std::string										_serverName;
		std::string										_rootDirectory;
		std::string										_index;
		int												_limitClientBodySize;
		// data_type									cgi_scripts
		std::unordered_map<int, std::string>			_errorPages;

		std::unordered_map<std::string, LocationBlock>	_locationBlocks;
};


#endif
