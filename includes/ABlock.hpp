

#pragma once

#include <string>
#include <map>
#include <vector>

class LocationBlock;
class ServerBlock;

class ABlock
{
public:
	ABlock();
	ABlock(ServerBlock &serverBlock);
	ABlock(const ABlock &other);
	ABlock &operator=(const ABlock &other);
	virtual ~ABlock();

	// setters
	void setRootDirectory(std::string rootDirectory);
	void addIndex(std::string index);
	void setClientMaxBodySize(int clientMaxBodySize);
	void addErrorPage(int statusCode, std::string uri);
	void setRedirection(int statusCode, std::string path);

	// getters
	std::vector<std::string> 	getPortsListeningOn() const;
	std::vector<std::string> 	getServerName() const;

	std::string 				getRootDirectory() const;
	std::vector<std::string>	getIndex() const;
	int 						getClientMaxBodySize() const;
	std::map<int, std::string> 	getErrorPages() const;
	std::pair<int, std::string> getRedirection() const;

protected:
	std::vector<std::string> _portsListeningOn;
	std::vector<std::string> _serverName;

	std::string 				_rootDirectory;
	std::vector<std::string> 	_index;
	int 						_clientMaxBodySize;
	std::map<int, std::string> 	_errorPages;
	std::pair<int, std::string> _redirection;
};

