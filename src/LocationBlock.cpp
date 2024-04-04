
#include "webserv.h"
#include <iostream>
#include <ostream>

LocationBlock::LocationBlock()
	: ABlock(), _autoindexStatus(false), _allowedMethods()
{
}

LocationBlock::LocationBlock(ServerBlock &serverBlock)
	: ABlock(serverBlock), _autoindexStatus(false), _allowedMethods()
{
}

LocationBlock::LocationBlock(const ServerBlock &serverBlock)
	: ABlock(serverBlock), _autoindexStatus(false), _allowedMethods()
{
}

LocationBlock::~LocationBlock()
{
}

LocationBlock::LocationBlock(const LocationBlock &other) : ABlock(other)
{
	*this = other;
}

LocationBlock &LocationBlock::operator=(const LocationBlock &other)
{
	if (this != &other)
	{
		// copy assignment of base class ABlock
		ABlock::operator=(other);

		this->_autoindexStatus = other._autoindexStatus;
		this->_allowedMethods = other._allowedMethods;
	}
	return *this;
}

void LocationBlock::setAutoindexStatus(bool status)
{
	this->_autoindexStatus = status;
}

void LocationBlock::addAllowedMethods(std::string method)
{
	this->_allowedMethods.push_back(method);
}

bool LocationBlock::getAutoindexStatus() const
{
	return this->_autoindexStatus;
}

std::vector<std::string> LocationBlock::getAllowedMethods() const
{
	return this->_allowedMethods;
}
