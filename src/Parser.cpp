
#include "webserv.h"

Parser::Parser(const std::string &filePath)
	: _filePath(filePath), _fileStream(filePath.c_str()), _tempLine(""),
	  _lineNum(1), _serverBlockNum(1), _locationBlockNum(1), _bracketPairing(0),
	  _tempServerBlock(), _tempLocationBlock(this->_tempServerBlock)
{}

/*
Server:		listen, server_name
Location:	autoindex, limit_except, cgi_pass
Both:		root, index, client_max_body_size, error_page, return
*/

Parser::~Parser()
{
}

Parser::Parser(const Parser &other)
{
	(void)other;
}

Parser &Parser::operator=(const Parser &other)
{
	(void)other;
	return *this;
}

void Parser::parseServerBlocks(std::vector<ServerBlock> &serverBlocks)
{
	// open the file
	if (this->_fileStream.is_open() == false)
	{
		std::string errorMsg =
			"Error: Cannot open config file " + this->_filePath;
		throw CustomException(errorMsg);
	}

	// parsing the server blocks
	while (std::getline(this->_fileStream, this->_tempLine))
	{
		// converts the line into a input string stream
		std::istringstream iss(this->_tempLine);
		std::string str1, str2, str3;

		// gets the 1st and 2nd element in the line
		iss >> str1 >> str2 >> str3;

		if (isSkippableLine(str1))
		{
			this->_lineNum++;
			continue;
		}
		if (str1 == "server" && str2 == "{" && str3.empty())
		{
			std::cout << std::endl
					  << HYELLOW "Creating server block "
					  << this->_serverBlockNum << RESET << std::endl;
			this->_locationBlockNum = 1;
			this->_lineNum++;
			this->_serverBlockNum++;
			this->_bracketPairing++;

			this->_tempServerBlock = ServerBlock();

			parseServerBlockDirectives(this->_tempServerBlock);

			serverBlocks.push_back(this->_tempServerBlock);
		}
		else
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): Server blocks must be in the enclosed in this "
				  "format:\nserver {\n...\n}";
			throw CustomException(ss.str());
		}
	}
}

/*
parse location blocks:
location [path] {
	directive1,
	...,
}
*/
void Parser::parseLocationBlocks(std::istringstream &iss)
{
	std::string path, str1, str2;

	// gets the 2nd and 3rd element in the line
	iss >> path >> str1 >> str2;

	if (str1 == "{" && str2.empty())
	{
		std::cout << HYELLOW "Creating location block "
				  << this->_locationBlockNum << " (" << path << ")" << RESET
				  << std::endl;

		this->_tempLocationBlock = LocationBlock(this->_tempServerBlock);

		this->_lineNum++;
		this->_locationBlockNum++;
		this->_bracketPairing++;

		parseLocationBlockDirectives(this->_tempLocationBlock);

		this->_tempServerBlock.addLocationBlock(path, this->_tempLocationBlock);
	}
	else
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
		   << "): Location blocks must be in the enclosed in this "
			  "format:\nlocation [path] {\n...\n}";
		throw CustomException(ss.str());
	}
}

// parses the individual directives like: listen, server_name and so on
void Parser::parseServerBlockDirectives(ServerBlock &block)
{
	while (std::getline(this->_fileStream, this->_tempLine))
	{
		if (isClosedCurlyBracket(this->_tempLine))
		{
			this->_lineNum++;
			this->_bracketPairing--;
			return;
		}
		if (isSkippableLine(this->_tempLine))
		{
			this->_lineNum++;
			continue;
		}
		if (!isLocationDirective(this->_tempLine))
		{
			if (isValidSemicolonFormat(this->_tempLine))
			{
				// removes the ; at the end of the line
				this->_tempLine = this->_tempLine.substr(0, this->_tempLine.length() - 1);
			}
			else
			{
				std::stringstream ss;
				ss << "Error (line " << this->_lineNum
				<< "): The directive line should end with one ;";
				throw CustomException(ss.str());
			}
		}

		std::istringstream iss(this->_tempLine);
		std::string directive;

		iss >> directive;
		if (directive == "listen")
		{
			parsePortsListeningOn(iss);
		}
		else if (directive == "server_name")
		{
			parseServerName(iss);
		}
		else if (directive == "root")
		{
			parseRoot(block, iss);
		}
		else if (directive == "index")
		{
			parseIndex(block, iss);
		}
		else if (directive == "client_max_body_size")
		{
			parseClientMaxBodySize(block, iss);
		}
		else if (directive == "error_page")
		{
			parseErrorPages(block, iss);
		}
		else if (directive == "return")
		{
			parseRedirection(block, iss);
		}
		else if (directive == "location")
		{
			parseLocationBlocks(iss);
		}
		else if (directive == "autoindex" || directive == "limit_except" || directive == "cgi_pass")
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): The directive " << directive <<
				  " can only exist in the location blocks";
			throw CustomException(ss.str());
		}
		else
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): The directive " << directive <<
				  " does not exist";
			throw CustomException(ss.str());
		}
		if (directive != "location")
			this->_lineNum++;
	}
	if (this->_bracketPairing != 0)
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): Server block is not closed with }";
		throw CustomException(ss.str());
	}
}

void Parser::parseLocationBlockDirectives(LocationBlock &block)
{
	while (std::getline(this->_fileStream, this->_tempLine))
	{
		if (isClosedCurlyBracket(this->_tempLine))
		{
			this->_lineNum++;
			this->_bracketPairing--;
			return;
		}
		if (isSkippableLine(this->_tempLine))
		{
			this->_lineNum++;
			continue;
		}
		if (!isLocationDirective(this->_tempLine))
		{
			if (isValidSemicolonFormat(this->_tempLine))
			{
				// removes the ; at the end of the line
				this->_tempLine = this->_tempLine.substr(0, this->_tempLine.length() - 1);
			}
			else
			{
				std::stringstream ss;
				ss << "Error (line " << this->_lineNum
				<< "): The directive line should end with one ;";
				throw CustomException(ss.str());
			}
		}

		std::istringstream iss(this->_tempLine);
		std::string directive;

		iss >> directive;
		if (directive == "listen" || directive == "server_name" || directive == "location")
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): The directive " << directive <<
				  " can only exist in the server blocks";
			throw CustomException(ss.str());
		}
		else if (directive == "root")
		{
			parseRoot(block, iss);
		}
		else if (directive == "index")
		{
			parseIndex(block, iss);
		}
		else if (directive == "client_max_body_size")
		{
			parseClientMaxBodySize(block, iss);
		}
		else if (directive == "error_page")
		{
			parseErrorPages(block, iss);
		}
		else if (directive == "return")
		{
			parseRedirection(block, iss);
		}
		else if (directive == "location")
		{
			parseLocationBlocks(iss);
		}
		else if (directive == "autoindex")
		{
			parseAutoindexStatus(iss);
		}
		else if (directive == "limit_except")
		{
			parseAllowedMethods(iss);
		}
		else if (directive == "cgi_pass")
		{
			parseCgiPassPath(iss);
		}
		else
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): The directive " << directive <<
				  " does not exist";
			throw CustomException(ss.str());
		}
		this->_lineNum++;
	}
	if (this->_bracketPairing != 1)
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): Location block is not closed with }";
		throw CustomException(ss.str());
	}
}

/*
checks:
- if a line ends with a ;
- if there's only one ;

ignores location blocks
*/
bool Parser::isValidSemicolonFormat(std::string &line)
{
	size_t semicolonPos = line.find(';');
	unsigned int lineLen = line.length();

	if (semicolonPos == lineLen - 1 && semicolonPos != std::string::npos)
		return (true);
	return (false);
}


/*
- check if there's at least 1 port
- check if the port's within range, and does not have any special symbols
- check if 

*/
void Parser::parsePortsListeningOn(std::istringstream &iss)
{
	std::string port;

	iss >> port;
	if (port.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): listen must have at least one port";
		throw CustomException(ss.str());
	}

	while (!port.empty())
	{
		if (isValidPort(port))
		{
			std::cout << CYAN "added port: " << port << RESET << std::endl;
			this->_tempServerBlock.addPortsListeningOn(port);
		}
		else
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
			   << "): Port " << port << " is invalid";
			throw CustomException(ss.str());
		}
		if (!(iss >> port))
			break;
	}
}

bool Parser::isValidPort(std::string &port)
{
	for (unsigned int i = 0; i < port.length(); i++)
	{
		if (isdigit(port[i]) == false)
			return (false);
	}

	int portNum = std::stoi(port);

	if (portNum < 0 || portNum > 65536)
		return (false);

	return (true);
}

void Parser::parseServerName(std::istringstream &iss)
{
	std::string serverName;

	iss >> serverName;
	this->_tempServerBlock.setServerName(serverName);
	std::cout << CYAN "set server name: " << serverName << RESET << std::endl;
}

template <typename T>
void Parser::parseRoot(T &block, std::istringstream &iss)
{
	std::string rootDirectory;

	iss >> rootDirectory;
	block.setRootDirectory(rootDirectory);
	std::cout << MAGENTA "set root directory: " << rootDirectory << RESET
			  << std::endl;
}

template <typename T>
void Parser::parseIndex(T &block, std::istringstream &iss)
{
	std::string index;

	iss >> index;
	block.setIndex(index);
	std::cout << MAGENTA "set index: " << index << RESET << std::endl;
}

template <typename T>
void Parser::parseClientMaxBodySize(T &block, std::istringstream &iss)
{
	int clientMaxBodySize;

	iss >> clientMaxBodySize;
	block.setClientMaxBodySize(clientMaxBodySize);
	std::cout << MAGENTA "set limit client body size: " << clientMaxBodySize
			  << RESET << std::endl;
}

template <typename T>
void Parser::parseErrorPages(T &block, std::istringstream &iss)
{
	int statusCode;
	std::string uri;

	iss >> statusCode >> uri;
	block.addErrorPage(statusCode, uri);
	std::cout << MAGENTA "added error page: " << statusCode << " " << uri
			  << RESET << std::endl;
}

template <typename T>
void Parser::parseRedirection(T &block, std::istringstream &iss)
{
	int statusCode;
	std::string path;

	iss >> statusCode >> path;
	block.setRedirection(statusCode, path);
	std::cout << MAGENTA "added redirection: " << statusCode << " " << path
			  << RESET << std::endl;
}

void Parser::parseAutoindexStatus(std::istringstream &iss)
{
	std::string status;

	iss >> status;
	if (status == "on")
	{
		this->_tempLocationBlock.setAutoindexStatus(true);
	}
	std::cout << CYAN "autoindex set to on" << RESET << std::endl;
}

void Parser::parseAllowedMethods(std::istringstream &iss)
{
	std::string method;

	while (iss >> method)
	{
		std::cout << CYAN "added method: " << method << RESET << std::endl;
		this->_tempLocationBlock.addAllowedMethods(method);
	}
}

void Parser::parseCgiPassPath(std::istringstream &iss)
{
	std::string path;

	iss >> path;
	this->_tempLocationBlock.setCgiPassPath(path);
	std::cout << CYAN "cgi pass path: " << path << RESET << std::endl;
}

bool Parser::isSkippableLine(std::string &line)
{
	std::istringstream iss(line);
	std::string temp;

	iss >> temp;
	if (temp.empty() || temp == "#" || temp == "")
	{
		return true;
	}
	return false;
}

bool Parser::isLocationDirective(std::string &line)
{
	std::istringstream iss(line);
	std::string temp;

	iss >> temp;
	if (temp == "location")
		return (true);
	return (false);
}

bool Parser::isClosedCurlyBracket(std::string &line)
{
	std::istringstream iss(line);
	std::string temp, temp1;

	iss >> temp >> temp1;
	if (temp == "}" && temp1.empty())
		return true;
	return false;
}
