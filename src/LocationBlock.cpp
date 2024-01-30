
#include "webserv.h"

LocationBlock::LocationBlock() {}

LocationBlock::~LocationBlock() {}

LocationBlock::LocationBlock( const LocationBlock& other ) {
	(void)other;
}

LocationBlock& LocationBlock::operator=( const LocationBlock& other ) {
	(void)other;
	return *this;
}
