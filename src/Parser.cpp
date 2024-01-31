
#include "webserv.h"

Parser::Parser( const std::string& filePath ): 
	_filePath(filePath),
	_fileStream(filePath),
	_tempLine(""),
	_lineNum(1),
	_serverBlockNum(1),
	_locationBlockNum(1),
	_tempServerBlock()
	// _tempLocationBlock()
{
	this->_executeDirectiveParsing["listen"] 					= &Parser::parsePortsListeningOn;
	this->_executeDirectiveParsing["server_name"] 				= &Parser::parseServerName;
	this->_executeDirectiveParsing["root"] 						= &Parser::parseRoot;
	this->_executeDirectiveParsing["index"]						= &Parser::parseIndex;
	this->_executeDirectiveParsing["client_max_body_size"]		= &Parser::parseLimitClientBodySize;
	this->_executeDirectiveParsing["cgi_script"] 				= &Parser::parseCGI;
	this->_executeDirectiveParsing["error_page"] 				= &Parser::parseErrorPages;
	this->_executeDirectiveParsing["location"] 					= &Parser::parseLocationBlocks;

	this->_executeDirectiveParsing["autoindex"]					= &Parser::parseAutoindexStatus;
	this->_executeDirectiveParsing["limit_except"]			= &Parser::parseAllowedMethods;
	this->_executeDirectiveParsing["return"]					= &Parser::parseRedirection;
	this->_executeDirectiveParsing["cgi_pass"]					= &Parser::parseCgiPassPath;
}

Parser::~Parser() {}

Parser::Parser( const Parser& other ) {
	(void)other;
}

Parser& Parser::operator=( const Parser& other ) {
	(void)other;
	return *this;
}

void Parser::parseServerBlocks( std::vector<ServerBlock>& serverBlocks ) {
	// open the file
	if (this->_fileStream.is_open() == false) {
		std::string errorMsg = "Error: Cannot open config file " + this->_filePath;
		throw CustomException(errorMsg);
	}

	// parsing the server blocks
	while (std::getline(this->_fileStream, this->_tempLine)) {
		// converts the line into a input string stream
		std::istringstream 	iss(this->_tempLine);
		std::string			str1, str2;
		
		// gets the 1st and 2nd element in the line
		iss >> str1 >> str2;
		// std::cout << "str1: " << str1 << std::endl;
		// std::cout << "str2: " << str2 << std::endl;

		if (isSkippableLine(str1)) {
			this->_lineNum++;
			continue;
		}
	
		if (str1 == "server" && str2 == "{") {
			std::cout << std::endl << HYELLOW "Creating server block " << this->_serverBlockNum << RESET << std::endl;
			this->_lineNum++;
			this->_serverBlockNum++;
			
			this->_tempServerBlock = ServerBlock();

			parseDirectives();

			serverBlocks.push_back(this->_tempServerBlock);
		} else {
			std::string errorMsg = "Error (line "
									+ std::to_string(this->_lineNum)
									+ "): Server blocks must be in the enclosed in this format:\nserver {\n...\n}";
			throw CustomException(errorMsg);
		}
	}	
}

// parses the individual directives like: listen, server_name and so on
void	Parser::parseDirectives() {
	while (std::getline(this->_fileStream, this->_tempLine)) {
		std::istringstream 	iss(this->_tempLine);
		std::string			directive;

		iss >> directive;

		if (isSkippableLine(directive)) {
			this->_lineNum++;
			continue;
		} else if (directive == "}") {
			return;
		}

		// check if the directive exists in the unordered map using an iterator:
		std::unordered_map<std::string, FuncPtr>::iterator it = this->_executeDirectiveParsing.find(directive);
		
		// if it does then execute the function pointer to parse the directive
		if (it != this->_executeDirectiveParsing.end()) {
			(this->*(it->second))(iss);
			this->_lineNum++;
		} else {
			std::string errorMsg = "Error ("
									+ this->_filePath
									+ ": line "
									+ std::to_string(this->_lineNum)
									+ "): The directive "
									+ directive
									+ " does not exist";
			throw CustomException(errorMsg);
		}
	}
}

void	Parser::parsePortsListeningOn( std::istringstream& iss ) {
	int port;

	while (iss >> port) {
		std::cout << MAGENTA "	added port: " << port << RESET <<std::endl;
		this->_tempServerBlock.addPortsListeningOn(port);
	}
}

void	Parser::parseServerName( std::istringstream& iss ) {
	std::string serverName;

	iss >> serverName;
	removeSemicolon(serverName);
	this->_tempServerBlock.setServerName(serverName);
	std::cout << CYAN "	set server name: " << serverName << RESET <<std::endl;
}

void	Parser::parseRoot( std::istringstream& iss ) {
	std::string rootDirectory;

	iss >> rootDirectory;
	removeSemicolon(rootDirectory);
	this->_tempServerBlock.setRootDirectory(rootDirectory);
	std::cout << MAGENTA "	set root directory: " << rootDirectory << RESET <<std::endl;
}

void	Parser::parseIndex( std::istringstream& iss ) {
	std::string index;

	iss >> index;
	removeSemicolon(index);
	this->_tempServerBlock.setIndex(index);
	std::cout << CYAN "	set index: " << index << RESET <<std::endl;
}

void	Parser::parseLimitClientBodySize( std::istringstream& iss ) {
	int limitClientBodySize;

	iss >> limitClientBodySize;
	this->_tempServerBlock.setLimitClientBodySize(limitClientBodySize);
	std::cout << MAGENTA "	set limit client body size: " << limitClientBodySize << RESET <<std::endl;
}

void	Parser::parseCGI( std::istringstream& iss ) {
	(void)iss;
}

void	Parser::parseErrorPages( std::istringstream& iss ) {
	int			statusCode;
	std::string	uri;

	iss >> statusCode >> uri;
	removeSemicolon(uri);
	this->_tempServerBlock.addErrorPage(statusCode, uri);
	std::cout << MAGENTA "	added error page: " << statusCode << " " << uri << RESET << std::endl;
}

void	Parser::parseAutoindexStatus( std::istringstream& iss ) {
	std::string status;

	iss >> status;
	removeSemicolon(status);
	if (status == "on") {
		this->_tempLocationBlock.setAutoindexStatus(true);
	}
	std::cout << MAGENTA "		autoindex set to on" << RESET << std::endl;
}

void	Parser::parseAllowedMethods( std::istringstream& iss ) {
	std::string method;

	while (iss >> method) {
		removeSemicolon(method);
		std::cout << MAGENTA "		added method: " << method << RESET <<std::endl;
		this->_tempLocationBlock.addAllowedMethods(method);
	}
}

void	Parser::parseRedirection( std::istringstream& iss ) {
	int 		statusCode;
	std::string	path;

	iss >> statusCode >> path;
	removeSemicolon(path);
	this->_tempLocationBlock.setRedirection(statusCode, path);
	std::cout << MAGENTA "		added redirection: " << statusCode << " " << path << RESET << std::endl;
}

void	Parser::parseCgiPassPath( std::istringstream& iss ) {
	std::string path;

	iss >> path;
	removeSemicolon(path);
	this->_tempLocationBlock.setCgiPassPath(path);
	std::cout << MAGENTA "		cgi pass path: " << path << RESET << std::endl;
}


/*
parse location blocks: 
location [path] {
	directive1,
	...,
}
*/
void	Parser::parseLocationBlocks( std::istringstream& iss ) {
	std::string	path, str;
		
	// gets the 2nd and 3rd element in the line
	iss >> path >> str;
	// std::cout << "path: " << path << std::endl;
	// std::cout << "str: " << str << std::endl;

	if (str == "{") {
		std::cout << HYELLOW "	Creating location block " << this->_locationBlockNum << " (" << path << ")" << RESET << std::endl;
		
		this->_tempLocationBlock = LocationBlock();

		this->_lineNum++;
		this->_locationBlockNum++;

		parseDirectives();

		this->_tempServerBlock.addLocationBlock(path, this->_tempLocationBlock);
	} else {
		std::string errorMsg = "Error (line "
								+ std::to_string(this->_lineNum)
								+ "): Location blocks must be in the enclosed in this format:\nlocation [path] {\n...\n}";
		throw CustomException(errorMsg);
	}
}

bool	Parser::isSkippableLine( std::string& line ) {
	if (line.empty() || line == "#" || line == "") {
		return true;
	}
	return false;
}

void	Parser::removeSemicolon( std::string& str ) {
	if (str.back() == ';') {
		str.pop_back();
	}
}

