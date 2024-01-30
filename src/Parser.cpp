
#include "webserv.h"

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser( const Parser& other ) {
	(void)other;
}

Parser& Parser::operator=( const Parser& other ) {
	(void)other;
	return *this;
}

void Parser::parseConfigFile( std::string fileName ) {
	(void)fileName;
}

