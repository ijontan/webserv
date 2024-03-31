
#include "ABlock.hpp"
#include "MethodIO.hpp"
#include "webserv.h"

ABlock::ABlock()
	: _portsListeningOn(), _serverName(), _rootDirectory(""), _index(),
	  _clientMaxBodySize(MAX_CONTENT_LENGTH), _errorPages(), _redirection()
{
}

ABlock::ABlock(ServerBlock &serverBlock)
	: _portsListeningOn(serverBlock.getPortsListeningOn()),
	  _serverName(serverBlock.getServerName()),
	  _rootDirectory(serverBlock.getRootDirectory()),
	  _index(serverBlock.getIndex()),
	  _clientMaxBodySize(serverBlock.getClientMaxBodySize()),
	  _errorPages(serverBlock.getErrorPages()),
	  _redirection(serverBlock.getRedirection())
{
}

ABlock::~ABlock()
{
}

ABlock::ABlock(const ABlock &other)	
	: _portsListeningOn(other.getPortsListeningOn()),
	  _serverName(other.getServerName()),
	  _rootDirectory(other.getRootDirectory()),
	  _index(other.getIndex()),
	  _clientMaxBodySize(other.getClientMaxBodySize()),
	  _errorPages(other.getErrorPages()),
	  _redirection(other.getRedirection())
{
	*this = other;
}

ABlock &ABlock::operator=(const ABlock &other)
{
	if (this != &other)
	{
		this->_portsListeningOn = other._portsListeningOn;
		this->_serverName = other._serverName;
		this->_rootDirectory = other._rootDirectory;
		this->_index = other._index;
		this->_clientMaxBodySize = other._clientMaxBodySize;
		this->_errorPages = other._errorPages;
		this->_redirection = other._redirection;
	}
	return *this;
}

void ABlock::setRootDirectory(std::string rootDirectory)
{
	this->_rootDirectory = rootDirectory;
}

void ABlock::addIndex(std::string index)
{
	this->_index.push_back(index);
}

void ABlock::setClientMaxBodySize(int clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void ABlock::addErrorPage(int statusCode, std::string uri)
{
	this->_errorPages[statusCode] = uri;
}

void ABlock::setRedirection(int statusCode, std::string path)
{
	this->_redirection.first = statusCode;
	this->_redirection.second = path;
}

std::vector<std::string> ABlock::getPortsListeningOn() const
{
	return this->_portsListeningOn;
}

std::vector<std::string> ABlock::getServerName() const
{
	return this->_serverName;
}

std::string ABlock::getRootDirectory() const
{
	return this->_rootDirectory;
}

std::vector<std::string> ABlock::getIndex() const
{
	return this->_index;
}

int ABlock::getClientMaxBodySize() const
{
	return this->_clientMaxBodySize;
}

std::map<int, std::string> ABlock::getErrorPages() const
{
	return this->_errorPages;
}

std::pair<int, std::string> ABlock::getRedirection() const
{
	return this->_redirection;
}
