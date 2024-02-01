
#include "webserv.h"

ServerBlock::ServerBlock():
	ABlock(),
	_locationBlocks()
{}

ServerBlock::~ServerBlock() {}

ServerBlock::ServerBlock( const ServerBlock& other ) {
	*this = other;
}

ServerBlock& ServerBlock::operator=( const ServerBlock& other ) {
	if (this != &other) {
		this->_locationBlocks = other._locationBlocks;
	}
	return *this;
}

void	ServerBlock::addPortsListeningOn( int port ) {
	this->_portsListeningOn.push_back(port);
}

void	ServerBlock::setServerName( std::string serverName ) {
	this->_serverName = serverName;
}

void 	ServerBlock::addLocationBlock( std::string path, LocationBlock locationBlock ) {
	this->_locationBlocks[path] = locationBlock;
}

std::ostream& operator<<(std::ostream& os, const ServerBlock& serverBlock) {
	os << "listen: ";
	const std::vector<int>& ports = serverBlock.getPortsListeningOn();

	for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); it++) {
		os << *it << " ";
	}
	os << std::endl;


	os << "server_name: " << serverBlock.getServerName() << std::endl;
	
	os << "root: " << serverBlock.getRootDirectory() << std::endl;
	os << "index: " << serverBlock.getIndex() << std::endl;
	os << "client_max_body_size: " << serverBlock.getClientMaxBodySize() << std::endl;
	// os << "error_pages: " << serverBlock.getRootDirectory() << std::endl;
	os << "redirection: " << serverBlock.getRootDirectory() << std::endl;

	return os;
}
