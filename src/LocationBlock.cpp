
#include "webserv.h"

LocationBlock::LocationBlock():
	_autoindexStatus(false),
	_allowedMethods(),
	_redirection(),
	_cgiPass("")
{}

LocationBlock::~LocationBlock() {}

LocationBlock::LocationBlock( const LocationBlock& other ) {
	*this = other;
}

LocationBlock& LocationBlock::operator=( const LocationBlock& other ) {
	if (this != &other) {
		this->_autoindexStatus = other._autoindexStatus;
		this->_allowedMethods = other._allowedMethods;
		this->_redirection = other._redirection;
		this->_cgiPass = other._cgiPass;
	}
	return *this;
}

void	LocationBlock::setAutoindexStatus( bool status ) {
	this->_autoindexStatus = status;
}

void	LocationBlock::addAllowedMethods( std::string method ) {
	this->_allowedMethods.push_back(method);
}

void	LocationBlock::setRedirection( int statusCode, std::string path ) {
	this->_redirection.first = statusCode;
	this->_redirection.second = path;
}

void	LocationBlock::setCgiPassPath( std::string path ) {
	this->_cgiPass = path;
}

