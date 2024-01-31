
#include "webserv.h"

LocationBlock::LocationBlock():
	// _autoindex(false),
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
		// this->_autoindex = other._autoindex;
		this->_allowedMethods = other._allowedMethods;
		this->_redirection = other._redirection;
		this->_cgiPass = other._cgiPass;
	}
	return *this;
}
