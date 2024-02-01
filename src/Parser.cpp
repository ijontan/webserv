
#include "webserv.h"

Parser::Parser( const std::string& filePath ): 
	_filePath(filePath),
	_fileStream(filePath),
	_tempLine(""),
	_lineNum(1),
	_serverBlockNum(1),
	_locationBlockNum(1),
	_tempServerBlock(),
	_tempLocationBlock(this->_tempServerBlock)
{
	// this->_executeDirectiveParsing["listen"] 					= &Parser::parsePortsListeningOn;
	// this->_executeDirectiveParsing["server_name"] 				= &Parser::parseServerName;

	// this->_executeDirectiveParsing["root"] 						= &Parser::parseRoot;
	// this->_executeDirectiveParsing["index"]						= &Parser::parseIndex;
	// this->_executeDirectiveParsing["client_max_body_size"]		= &Parser::parseClientMaxBodySize;
	// this->_executeDirectiveParsing["error_page"] 				= &Parser::parseErrorPages;
	// this->_executeDirectiveParsing["return"]					= &Parser::parseRedirection;

	// this->_executeDirectiveParsing["location"] 					= &Parser::parseLocationBlocks;

	// this->_executeDirectiveParsing["autoindex"]					= &Parser::parseAutoindexStatus;
	// this->_executeDirectiveParsing["limit_except"]				= &Parser::parseAllowedMethods;
	// this->_executeDirectiveParsing["cgi_pass"]					= &Parser::parseCgiPassPath;
}

/*
Server:		listen, server_name
Location:	autoindex, limit_except, cgi_pass
Both:		root, index, client_max_body_size, error_page, return
*/

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
			this->_locationBlockNum = 1;
			this->_lineNum++;
			this->_serverBlockNum++;

			this->_tempServerBlock = ServerBlock();

			parseDirectives(this->_tempServerBlock);
			std::cout << this->_tempServerBlock << std::endl;


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
template <typename T>
void	Parser::parseDirectives( T& block ) {
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

		// // check if the directive exists in the unordered map using an iterator:
		// std::unordered_map<std::string, FuncPtr>::iterator it = this->_executeDirectiveParsing.find(directive);

		// // if it does then execute the function pointer to parse the directive
		// if (it != this->_executeDirectiveParsing.end()) {
		// 	(this->*(it->second))(iss);
		// 	this->_lineNum++;
		// } else {
		// 	std::string errorMsg = "Error ("
		// 							+ this->_filePath
		// 							+ ": line "
		// 							+ std::to_string(this->_lineNum)
		// 							+ "): The directive "
		// 							+ directive
		// 							+ " does not exist";
		// 	throw CustomException(errorMsg);
		// }

		if (directive == "listen") {
			parsePortsListeningOn(iss);
		} else if (directive == "server_name") {
			parseServerName(iss);
		} else if (directive == "root") {
			parseRoot(block, iss);
		} else if (directive == "index") {
			parseIndex(block, iss);
		} else if (directive == "client_max_body_size") {
			parseClientMaxBodySize(block, iss);
		} else if (directive == "error_page") {
			parseErrorPages(block, iss);
		} else if (directive == "return") {
			parseRedirection(block, iss);
		} else if (directive == "location") {
			parseLocationBlocks(iss);
		} else if (directive == "autoindex") {
			parseAutoindexStatus(iss);
		} else if (directive == "limit_except") {
			parseAllowedMethods(iss);
		} else if (directive == "cgi_pass") {
			parseCgiPassPath(iss);
		}
	}
}


void	Parser::parsePortsListeningOn( std::istringstream& iss ) {
	std::string port;

	while (iss >> port) {
		std::cout << CYAN "added port: " << port << RESET <<std::endl;
		this->_tempServerBlock.addPortsListeningOn(port);
	}
}

void	Parser::parseServerName( std::istringstream& iss ) {
	std::string serverName;

	iss >> serverName;
	removeSemicolon(serverName);
	this->_tempServerBlock.setServerName(serverName);
	std::cout << CYAN "set server name: " << serverName << RESET <<std::endl;
}

template <typename T>
void	Parser::parseRoot( T& block, std::istringstream& iss ) {
	std::string rootDirectory;

	iss >> rootDirectory;
	removeSemicolon(rootDirectory);
	block.setRootDirectory(rootDirectory);
	std::cout << MAGENTA "set root directory: " << rootDirectory << RESET <<std::endl;
}

template <typename T>
void	Parser::parseIndex( T& block, std::istringstream& iss ) {
	std::string index;

	iss >> index;
	removeSemicolon(index);
	block.setIndex(index);
	std::cout << MAGENTA "set index: " << index << RESET <<std::endl;
}

template <typename T>
void	Parser::parseClientMaxBodySize( T& block, std::istringstream& iss ) {
	int clientMaxBodySize;

	iss >> clientMaxBodySize;
	block.setClientMaxBodySize(clientMaxBodySize);
	std::cout << MAGENTA "set limit client body size: " << clientMaxBodySize << RESET <<std::endl;
}

template <typename T>
void	Parser::parseErrorPages( T& block, std::istringstream& iss ) {
	int			statusCode;
	std::string	uri;

	iss >> statusCode >> uri;
	removeSemicolon(uri);
	block.addErrorPage(statusCode, uri);
	std::cout << MAGENTA "added error page: " << statusCode << " " << uri << RESET << std::endl;
}

template <typename T>
void	Parser::parseRedirection( T& block, std::istringstream& iss ) {
	int 		statusCode;
	std::string	path;

	iss >> statusCode >> path;
	removeSemicolon(path);
	block.setRedirection(statusCode, path);
	std::cout << MAGENTA "added redirection: " << statusCode << " " << path << RESET << std::endl;
}

void	Parser::parseAutoindexStatus( std::istringstream& iss ) {
	std::string status;

	iss >> status;
	removeSemicolon(status);
	if (status == "on") {
		this->_tempLocationBlock.setAutoindexStatus(true);
	}
	std::cout << CYAN "autoindex set to on" << RESET << std::endl;
}

void	Parser::parseAllowedMethods( std::istringstream& iss ) {
	std::string method;

	while (iss >> method) {
		removeSemicolon(method);
		std::cout << CYAN "added method: " << method << RESET <<std::endl;
		this->_tempLocationBlock.addAllowedMethods(method);
	}
}

void	Parser::parseCgiPassPath( std::istringstream& iss ) {
	std::string path;

	iss >> path;
	removeSemicolon(path);
	this->_tempLocationBlock.setCgiPassPath(path);
	std::cout << CYAN "cgi pass path: " << path << RESET << std::endl;
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
		std::cout << HYELLOW "Creating location block " << this->_locationBlockNum << " (" << path << ")" << RESET << std::endl;
		
		this->_tempLocationBlock = LocationBlock(this->_tempServerBlock);

		this->_lineNum++;
		this->_locationBlockNum++;

		parseDirectives(this->_tempLocationBlock);

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

