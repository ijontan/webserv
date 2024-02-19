
#include "webserv.h"

#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

class LocationBlock : public ABlock
{
public:
	LocationBlock();
	LocationBlock(ServerBlock &serverBlock);
	LocationBlock(const LocationBlock &other);
	LocationBlock &operator=(const LocationBlock &other);
	~LocationBlock();

	// setters
	void setAutoindexStatus(bool status);
	void addAllowedMethods(std::string path);
	void setCgiPassPath(std::string path);

	// getters
	bool getAutoindexStatus() const;
	std::vector<std::string> getAllowedMethods() const;
	std::string getCgiPass() const;

private:
	bool _autoindexStatus;
	std::vector<std::string> _allowedMethods;
	std::string _cgiPass;
};

#endif
