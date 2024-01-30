
#include "webserv.h"

ServerBlock::ServerBlock() {}

ServerBlock::~ServerBlock() {}

ServerBlock::ServerBlock( const ServerBlock& other ) {
	*this = other;
}

ServerBlock& ServerBlock::operator=( const ServerBlock& other ) {
	if (this != &other) {
		this->_portsListeningOn = other._portsListeningOn;
		this->_serverName = other._serverName;
		this->_rootDirectory = other._rootDirectory;
		this->_index = other._index;

		// this->_limitClientBodySize = other._limitClientBodySize;
		// cgis

		this->_locationBlocks = other._locationBlocks;
	}
	return *this;
}

