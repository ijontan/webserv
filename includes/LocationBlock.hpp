
#pragma once

#include "ABlock.hpp"

class LocationBlock : public ABlock
{
public:
	LocationBlock();
	LocationBlock(ServerBlock &serverBlock);
	LocationBlock(const ServerBlock &serverBlock);
	LocationBlock(const LocationBlock &other);
	LocationBlock &operator=(const LocationBlock &other);
	~LocationBlock();

	// setters
	void setAutoindexStatus(bool status);
	void addAllowedMethods(std::string path);

	// getters
	bool getAutoindexStatus() const;
	std::vector<std::string> getAllowedMethods() const;

private:
	bool _autoindexStatus;
	std::vector<std::string> _allowedMethods;
};

