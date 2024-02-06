
#include "webserv.h"

LocationBlock::LocationBlock() : ABlock(),
								 _autoindexStatus(false),
								 _allowedMethods(),
								 _cgiPass("")
{
}

LocationBlock::LocationBlock(ServerBlock &serverBlock) : ABlock(serverBlock),
														 _autoindexStatus(false),
														 _allowedMethods(),
														 _cgiPass("")
{
}

LocationBlock::~LocationBlock() {}

LocationBlock::LocationBlock(const LocationBlock &other) : ABlock()
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
		this->_cgiPass = other._cgiPass;
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

void LocationBlock::setCgiPassPath(std::string path)
{
	this->_cgiPass = path;
}

bool LocationBlock::getAutoindexStatus() const
{
	return this->_autoindexStatus;
}

std::vector<std::string> LocationBlock::getAllowedMethods() const
{
	return this->_allowedMethods;
}

std::string LocationBlock::getCgiPass() const
{
	return this->_cgiPass;
}
