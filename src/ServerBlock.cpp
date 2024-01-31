
#include "webserv.h"

ServerBlock::ServerBlock():
	_portsListeningOn(),
	_serverName(""),
	_rootDirectory(""),
	_index(""),
	// _limitClientBodySize(0),
	// cgi,
	_errorPages(),
	_locationBlocks()
{}

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
		this->_limitClientBodySize = other._limitClientBodySize;
		// cgis

		this->_locationBlocks = other._locationBlocks;
	}
	return *this;
}

void	ServerBlock::setPortsListeningOn( std::vector<int> portsListeningOn ) {
	this->_portsListeningOn = portsListeningOn;
}

void	ServerBlock::setServerName( std::string serverName ) {
	this->_serverName = serverName;
}

void	ServerBlock::setRootDirectory( std::string rootDirectory ) {
	this->_rootDirectory = rootDirectory;
}

void	ServerBlock::setIndex( std::string index ) {
	this->_index = index;
}

void	ServerBlock::setLimitClientBodySize( int limitClientBodySize ) {
	this->_limitClientBodySize = limitClientBodySize;
}

// data_type	cgi_scripts

void 	ServerBlock::addErrorPage( int statusCode, std::string uri ) {
	this->_errorPages[statusCode] = uri;
}

void 	ServerBlock::setLocationBlocks( std::vector<LocationBlock> locationBlocks ) {
	this->_locationBlocks = locationBlocks;
}

