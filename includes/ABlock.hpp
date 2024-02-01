
#include "webserv.h"

#ifndef ABLOCK_HPP
#define ABLOCK_HPP

class LocationBlock;
class ServerBlock;

class ABlock {
	public:
		ABlock();
		ABlock( ServerBlock& serverBlock );
		ABlock( const ABlock& other );
		ABlock& operator=( const ABlock& other );
		virtual ~ABlock();

		// setters
		void	setRootDirectory( std::string rootDirectory );
		void	setIndex( std::string index );
		void	setClientMaxBodySize( int clientMaxBodySize );
		void 	addErrorPage( int statusCode, std::string uri );
		void	setRedirection( int statusCode, std::string path );

		// getters
		std::vector<int>						getPortsListeningOn() const;
		std::string								getServerName() const;
		
		std::string								getRootDirectory() const;
		std::string								getIndex() const;
		int										getClientMaxBodySize() const;
		std::unordered_map<int, std::string>	getErrorPages() const;
		std::pair<int, std::string>				getRedirection() const;

	protected:
		std::vector<int>								_portsListeningOn;
		std::string										_serverName;

		std::string										_rootDirectory;
		std::string										_index;
		int												_clientMaxBodySize;
		std::unordered_map<int, std::string>			_errorPages;
		std::pair<int, std::string>						_redirection;
};


#endif
