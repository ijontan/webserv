
#include "Webserv.hpp"

#ifndef PARSER_HPP
#define PARSER_HPP

class Parser {
	public:
		Parser();
		~Parser();

		void parseConfigFile( std::string fileName );

	private:
		Parser( const Parser& other );
		Parser& operator=( const Parser& other );

		std::vector<int>	_portsListeningOn;
		std::string			_serverName;
		std::string			_rootDirectory;
		std::string			_index;
		// data_type		cgi_scripts
		
};


#endif
