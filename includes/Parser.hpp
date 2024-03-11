
#pragma once

#include "LocationBlock.hpp"
#include "ServerBlock.hpp"
#include <fstream>

class Parser
{
public:
	Parser(const std::string &filePath);
	~Parser();

	typedef void (Parser::*FuncPtr)(std::istringstream &);

	// parsing the server block
	void parseServerBlocks(std::vector<ServerBlock> &serverBlocks);
	void parseServerBlockDirectives(ServerBlock &block);
	void parseLocationBlockDirectives(LocationBlock &block);

	void parsePortsListeningOn(std::istringstream &iss);
	void parseServerName(std::istringstream &iss);

	template <typename T>
	void parseRoot(T &block, std::istringstream &iss);
	template <typename T>
	void parseIndex(T &block, std::istringstream &iss);
	template <typename T>
	void parseClientMaxBodySize(T &block, std::istringstream &iss);
	template <typename T>
	void parseErrorPages(T &block, std::istringstream &iss);
	template <typename T>
	void parseRedirection(T &block, std::istringstream &iss);

	void parseAutoindexStatus(std::istringstream &iss);
	void parseAllowedMethods(std::istringstream &iss);
	void parseCgiPassPath(std::istringstream &iss);

	// parsing the location block
	void parseLocationBlocks(std::istringstream &iss);

	// utils
	bool isSkippableLine(std::string &line);

	// error checking
	bool isValidSemicolonFormat(std::string &line);
	bool isLocationDirective(std::string &line);
	bool isClosedCurlyBracket(std::string &line);
	bool isValidPort(std::string &port);
	bool isValidErrorStatusCode(int statusCode);
	bool isValidMethod(std::string &method);
	bool isValidNumber(std::string &num);
	bool isUniqueServerName(std::string &serverName);

private:
	Parser(const Parser &other);
	Parser &operator=(const Parser &other);

	// variables for parsing
	std::string _filePath;
	std::ifstream _fileStream;
	std::string _tempLine;
	int _lineNum;
	int _serverBlockNum;
	int _locationBlockNum;
	int _bracketPairing;
	bool _isFileEmpty;
	bool _hasDirectives;
	std::vector<std::string> _serverNames;
	ServerBlock _tempServerBlock;
	LocationBlock _tempLocationBlock;
};

