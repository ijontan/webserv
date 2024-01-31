
#include "webserv.h"

#ifndef PARSER_HPP
#define PARSER_HPP

#include "ServerBlock.hpp"

class Parser {
	public:
		Parser( const std::string& filePath );
		~Parser();

		typedef void (Parser::*FuncPtr)( std::istringstream& );

		void	parseServerBlocks( std::vector<ServerBlock>& serverBlocks );
		void	parseServerBlockDirectives();
		
		// parsing the server block's directives
		void	parsePortsListeningOn( std::istringstream& iss );
		void	parseServerName( std::istringstream& iss );
		void	parseRoot( std::istringstream& iss );
		void	parseIndex( std::istringstream& iss );
		void	parseLimitClientBodySize( std::istringstream& iss );
		void	parseCGI( std::istringstream& iss );
		void	parseErrorPages( std::istringstream& iss );
		void	parseLocationBlocks( std::istringstream& iss );

		// utils
		bool	isSkippableLine( std::string& line );

	private:
		Parser( const Parser& other );
		Parser& operator=( const Parser& other );

		// variables for parsing
		std::string									_filePath;
		std::ifstream								_fileStream;
		std::string									_tempLine;
		int											_lineNum;
		int											_serverBlockNum;
		ServerBlock									_tempServerBlock;
		// LocationBlock								_tempLocationBlock;

		std::unordered_map<std::string, FuncPtr>	_executeDirectiveParsing;
};


#endif
