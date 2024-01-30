
#include "webserv.h"

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
};


#endif
