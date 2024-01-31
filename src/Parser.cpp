
#include "webserv.h"

Parser::Parser( const std::string& filePath ): 
	_filePath(filePath),
	_fileStream(filePath),
	_tempLine(""),
	_lineNum(1),
	_serverBlockNum(1),
	_tempServerBlock()
	// _tempLocationBlock()
{
	this->_executeDirectiveParsing["listen"] 					= &Parser::parsePortsListeningOn;
	this->_executeDirectiveParsing["server_name"] 				= &Parser::parseServerName;
	this->_executeDirectiveParsing["root"] 						= &Parser::parseRoot;
	this->_executeDirectiveParsing["index"]						= &Parser::parseIndex;
	this->_executeDirectiveParsing["limit_client_body_size"]	= &Parser::parseLimitClientBodySize;
	this->_executeDirectiveParsing["cgi_script"] 				= &Parser::parseCGI;
	this->_executeDirectiveParsing["error_page"] 				= &Parser::parseErrorPages;
	this->_executeDirectiveParsing["location"] 					= &Parser::parseLocationBlocks;
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
		if (isSkippableLine(this->_tempLine)) {
			this->_lineNum++;
			continue;
		}

		// converts the line into a input string stream
		std::istringstream 	iss(this->_tempLine);
		std::string			directive1, directive2;
		
		// gets the 1st and 2nd element in the line
		iss >> directive1 >> directive2;
		// std::cout << "directive1: " << directive1 << std::endl;
		// std::cout << "directive2: " << directive2 << std::endl;

		if (directive1 == "server" && directive2 == "{") {
			std::cout << HYELLOW "Creating server block " << this->_serverBlockNum << RESET << std::endl;
			
			this->_tempServerBlock = ServerBlock();

			this->_lineNum++;
		
			parseServerBlockDirectives();

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
void	Parser::parseServerBlockDirectives() {
	while (std::getline(this->_fileStream, this->_tempLine)) {
		if (isSkippableLine(this->_tempLine)) {
			this->_lineNum++;
			continue;
		}

		std::istringstream 	iss(this->_tempLine);
		std::string			directive;

		iss >> directive;

		std::unordered_map<std::string, FuncPtr>::iterator it = this->_executeDirectiveParsing.find(directive);
		
		// check if the directive exists in the unordered map:
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
	std::vector<int> portsListeningOn;
	int portNum;

	while (iss >> portNum) {
		std::cout << MAGENTA "added port: " << portNum << RESET <<std::endl;
		portsListeningOn.push_back(portNum);
	}
	this->_tempServerBlock.setPortsListeningOn(portsListeningOn);
}

void	Parser::parseServerName( std::istringstream& iss ) {
	std::string serverName;

	iss >> serverName;
	if (serverName.back() == ';') {
		serverName.pop_back();
	}
	this->_tempServerBlock.setServerName(serverName);
	std::cout << CYAN "set server name: " << serverName << RESET <<std::endl;
}

void	Parser::parseRoot( std::istringstream& iss ) {
	std::string rootDirectory;

	iss >> rootDirectory;
	if (rootDirectory.back() == ';') {
		rootDirectory.pop_back();
	}
	this->_tempServerBlock.setRootDirectory(rootDirectory);
	std::cout << MAGENTA "set root directory: " << rootDirectory << RESET <<std::endl;
}

void	Parser::parseIndex( std::istringstream& iss ) {
	std::string index;

	iss >> index;
	if (index.back() == ';') {
		index.pop_back();
	}
	this->_tempServerBlock.setIndex(index);
	std::cout << CYAN "set index: " << index << RESET <<std::endl;
}

void	Parser::parseLimitClientBodySize( std::istringstream& iss ) {
	int limitClientBodySize;

	iss >> limitClientBodySize;
	this->_tempServerBlock.setLimitClientBodySize(limitClientBodySize);
	std::cout << MAGENTA "set limit client body size: " << limitClientBodySize << RESET <<std::endl;
}

void	Parser::parseCGI( std::istringstream& iss ) {
	(void)iss;
}

void	Parser::parseErrorPages( std::istringstream& iss ) {
	int			statusCode;
	std::string	uri;

	iss >> statusCode >> uri;
	if (uri.back() == ';') {
		uri.pop_back();
	}
	this->_tempServerBlock.addErrorPage(statusCode, uri);
	std::cout << MAGENTA "added error page: " << statusCode << " " << uri << RESET << std::endl;
}

void	Parser::parseLocationBlocks( std::istringstream& iss ) {
	(void)iss;
}

bool	Parser::isSkippableLine( std::string& line ) {
	std::istringstream	iss(line);
	std::string			str;

	iss >> str;
	if (line.empty() || line.find('#') == 0 || str == "") {
		return true;
	}
	return false;
}

