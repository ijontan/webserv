
#include "webserv.h"

#ifndef PARSER_HPP
#define PARSER_HPP

class ServerBlock;

class Parser {
	public:
		Parser();
		~Parser();

		std::vector<ServerBlock> createServerBlocks( std::string filePath );

	private:
		Parser( const Parser& other );
		Parser& operator=( const Parser& other );
};


#endif
