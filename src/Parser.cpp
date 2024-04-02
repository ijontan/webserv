
#include "webserv.h"

Parser::Parser(const std::string &filePath)
	: _filePath(filePath), _fileStream(filePath.c_str()), _tempLine(""),
	  _lineNum(1), _serverBlockNum(1), _locationBlockNum(1), _bracketPairing(0),
	  _isFileEmpty(true), _hasDirectives(false), _serverNames(), 
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
			this->_isFileEmpty = false;

			this->_tempServerBlock = ServerBlock();

			initServerDirectiveCount();
			parseServerBlockDirectives(this->_tempServerBlock);
			checkServerDirectiveCount();

			// checks if the server block has directives
			if (this->_hasDirectives == false)
				throw CustomException("Error: Server block cannot be empty\nserver {\n  directive1\n  directive2\n  ...\n}");

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
	// checks if the file is empty or not
	if (this->_isFileEmpty == true)
	{
		std::stringstream ss;
		ss << "Error: " << this->_filePath << " is empty";
		throw CustomException(ss.str());
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

		initLocationDirectiveCount();
		parseLocationBlockDirectives(this->_tempLocationBlock);
		checkLocationDirectiveCount();

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
			this->_serverDirectiveCount["listen"]++;
		}
		else if (directive == "server_name")
		{
			parseServerName(iss);
			this->_serverDirectiveCount["server_name"]++;
		}
		else if (directive == "root")
		{
			parseRoot(block, iss);
			this->_serverDirectiveCount["root"]++;
		}
		else if (directive == "index")
		{
			parseIndex(block, iss);
			this->_serverDirectiveCount["index"]++;
		}
		else if (directive == "client_max_body_size")
		{
			parseClientMaxBodySize(block, iss);
			this->_serverDirectiveCount["client_max_body_size"]++;
		}
		else if (directive == "error_page")
		{
			parseErrorPages(block, iss);
			this->_serverDirectiveCount["error_page"]++;
		}
		else if (directive == "return")
		{
			parseRedirection(block, iss);
			this->_serverDirectiveCount["return"]++;
		}
		else if (directive == "location")
		{
			parseLocationBlocks(iss);
		}
		else if (directive == "autoindex" || directive == "limit_except")
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
		this->_hasDirectives = true;
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
		if (directive == "root")
		{
			parseRoot(block, iss);
			this->_locationDirectiveCount["root"]++;
		}
		else if (directive == "index")
		{
			parseIndex(block, iss);
			this->_locationDirectiveCount["index"]++;
		}
		else if (directive == "client_max_body_size")
		{
			parseClientMaxBodySize(block, iss);
			this->_locationDirectiveCount["client_max_body_size"]++;
		}
		else if (directive == "error_page")
		{
			parseErrorPages(block, iss);
			this->_locationDirectiveCount["error_page"]++;
		}
		else if (directive == "return")
		{
			parseRedirection(block, iss);
			this->_locationDirectiveCount["return"]++;
		}
		else if (directive == "autoindex")
		{
			parseAutoindexStatus(iss);
			this->_locationDirectiveCount["autoindex"]++;
		}
		else if (directive == "limit_except")
		{
			parseAllowedMethods(iss);
			this->_locationDirectiveCount["limit_except"]++;
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
*/
void Parser::parsePortsListeningOn(std::istringstream &iss)
{
	std::string port;

	iss >> port;
	if (port.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): listen [port1] [port2] ... (needs at least one port number)";
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

void Parser::parseServerName(std::istringstream &iss)
{
	std::string serverName;

	iss >> serverName;
	if (serverName.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): server_name [serverName1] [serverName2] ... (needs at least one server name)";
		throw CustomException(ss.str());
	}
	while (!serverName.empty())
	{
		if (!isUniqueServerName(serverName))
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
				<< "): server name duplicate found (all server names have to be unique)";
			throw CustomException(ss.str());
		}
		this->_tempServerBlock.addServerName(serverName);
		this->_serverNames.push_back(serverName);
		std::cout << CYAN "added server name: " << serverName << RESET << std::endl;
		if (!(iss >> serverName))
			break;
	}
}

template <typename T>
void Parser::parseRoot(T &block, std::istringstream &iss)
{
	std::string rootDirectory;
	std::string temp;

	iss >> rootDirectory >> temp;
	if (rootDirectory.empty() || !temp.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): root [rootDirectoryPath] (needs only one absolute path)";
		throw CustomException(ss.str());
	}
	block.setRootDirectory(rootDirectory);
	std::cout << MAGENTA "set root directory: " << rootDirectory << RESET
			  << std::endl;
}

template <typename T>
void Parser::parseIndex(T &block, std::istringstream &iss)
{
	std::string index;

	iss >> index;
	if (index.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): index [index1] [index2] ... (needs at least one relative path)";
		throw CustomException(ss.str());
	}
	while (!index.empty())
	{
		block.addIndex(index);
		std::cout << MAGENTA "added index: " << index << RESET << std::endl;
		if (!(iss >> index))
			break;
	}
}

template <typename T>
void Parser::parseClientMaxBodySize(T &block, std::istringstream &iss)
{
	std::string clientMaxBodySize;
	std::string temp;
	int num;

	iss >> clientMaxBodySize >> temp;
	if (clientMaxBodySize.empty() || !isValidNumber(clientMaxBodySize) || !temp.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): client_body_max_size [int] (needs only one integer)";
		throw CustomException(ss.str());
	}
	num = utils::stoi(clientMaxBodySize);
	block.setClientMaxBodySize(num);
	std::cout << MAGENTA "set limit client body size: " << num
			  << RESET << std::endl;
}

template <typename T>
void Parser::parseErrorPages(T &block, std::istringstream &iss)
{
	int statusCode;
	std::string uri;
	std::string temp;

	iss >> statusCode >> uri >> temp;
	if (isValidErrorStatusCode(statusCode) && !uri.empty() && temp.empty())
	{
		block.addErrorPage(statusCode, uri);
		std::cout << MAGENTA "added error page: " << statusCode << " " << uri
				<< RESET << std::endl;
	}
	else
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): error_page [errorStatusCode] [filePath] (needs one status code (400, 403, 404, 408, 409, 415, 500) \
and one path)";
		throw CustomException(ss.str());
	}
}

template <typename T>
void Parser::parseRedirection(T &block, std::istringstream &iss)
{
	int statusCode;
	std::string path;
	std::string temp;

	statusCode = 0;
	iss >> statusCode >> path >> temp;
	// statusCode checking subject to change
	if (!statusCode || path.empty() || !temp.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): return [statusCode] [url]";
		throw CustomException(ss.str());
	}
	block.setRedirection(statusCode, path);
	std::cout << MAGENTA "added redirection: " << statusCode << " " << path
			  << RESET << std::endl;
}

void Parser::parseAutoindexStatus(std::istringstream &iss)
{
	std::string status;
	std::string temp;

	iss >> status >> temp;
	if (status.empty() || !temp.empty() || status != "on")
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): autoindex on (needs the on status)";
		throw CustomException(ss.str());
	}
	this->_tempLocationBlock.setAutoindexStatus(true);
	std::cout << CYAN "autoindex set to on" << RESET << std::endl;
}

void Parser::parseAllowedMethods(std::istringstream &iss)
{
	std::string method;

	iss >> method;
	if (method.empty())
	{
		std::stringstream ss;
		ss << "Error (line " << this->_lineNum
			<< "): limit_except [method1] [method2] ... (needs at least one method: GET, POST, DELETE)";
		throw CustomException(ss.str());
	}
	while (!method.empty())
	{
		if (isValidMethod(method))
		{
			std::cout << CYAN "added method: " << method << RESET << std::endl;
			this->_tempLocationBlock.addAllowedMethods(method);
		}
		else
		{
			std::stringstream ss;
			ss << "Error (line " << this->_lineNum
				<< "): " << method << " is an invalid method (valid methods: GET, POST, DELETE)";
			throw CustomException(ss.str());
		}
		if (!(iss >> method))
			break;
	}
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

bool Parser::isValidPort(std::string &port)
{
	if (!isValidNumber(port))
	{
		return (false);
	}

	int portNum = utils::stoi(port);

	if (portNum < 0 || portNum > 65536)
		return (false);

	return (true);
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

bool Parser::isValidErrorStatusCode(int statusCode)
{
	int validStatusCodes[7] = {400, 403, 404, 408, 409, 415, 500};
	
	for (int i = 0; i < 7; i++)
	{
		if (statusCode == validStatusCodes[i])
		{
			return (true);
		}
	}
	return (false);
}

bool Parser::isValidMethod(std::string &method)
{
	std::string validMethods[3] = {"GET", "POST", "DELETE"};
	
	for (int i = 0; i < 3; i++)
	{
		if (method == validMethods[i])
		{
			return (true);
		}
	}
	return (false);
}

bool Parser::isValidNumber(std::string &num)
{
	for (unsigned int i = 0; i < num.length(); i++)
	{
		if (isdigit(num[i]) == false)
			return (false);
	}
	return (true);
}

bool Parser::isUniqueServerName(std::string &serverName)
{
	if (this->_serverNames.empty())
		return true;
	for (unsigned int i = 0; i < this->_serverNames.size(); i++)
	{
		if (serverName == this->_serverNames[i])
			return false;
	}
	return true;
}

void Parser::initServerDirectiveCount()
{
	this->_serverDirectiveCount["listen"] = 0;
	this->_serverDirectiveCount["server_name"] = 0;
	this->_serverDirectiveCount["root"] = 0;
	this->_serverDirectiveCount["index"] = 0;
	this->_serverDirectiveCount["client_max_body_size"] = 0;
	this->_serverDirectiveCount["error_page"] = 0;
	this->_serverDirectiveCount["return"] = 0;
}

void Parser::initLocationDirectiveCount()
{
	this->_locationDirectiveCount["root"] = 0;
	this->_locationDirectiveCount["index"] = 0;
	this->_locationDirectiveCount["client_max_body_size"] = 0;
	this->_locationDirectiveCount["return"] = 0;
	this->_locationDirectiveCount["error_page"] = 0;
	this->_locationDirectiveCount["autoindex"] = 0;
	this->_locationDirectiveCount["limit_except"] = 0;
}

void Parser::checkServerDirectiveCount()
{
	std::stringstream ss;
	std::vector<std::string> directives;
	directives.push_back("listen");
	directives.push_back("server_name");
	directives.push_back("root");
	directives.push_back("index");
	directives.push_back("client_max_body_size");

	// check if 5 directives only have one of each
	for (int i = 0; i < 5; i++)
	{
		int count = _serverDirectiveCount[directives[i]];
		// std::cout << "directive: " << directives[i] << std::endl;
		// std::cout << "count: " << count << std::endl << std::endl;
		if (count == 0) {	
			ss << "Error (server block " << _serverBlockNum - 1 << "): The directive " << directives[i] << " is missing";
			throw CustomException(ss.str());
		} else if (count > 1) {
			ss << "Error (server block " << _serverBlockNum - 1 << "): The directive " << directives[i] << " can only be used once";
			throw CustomException(ss.str());
		}
	}

	// check if there's at least one error_page directive
	if (_serverDirectiveCount["error_page"] < 1)
	{
		ss << "Error (server block " << _serverBlockNum - 1 << "): There must be at least one error_page directive";
		throw CustomException(ss.str());
	}

	// check if there's no more than one return directive
	if (_serverDirectiveCount["return"] > 1)
	{
		ss << "Error (server block " << _serverBlockNum - 1 << "): There can only be one return directive";
		throw CustomException(ss.str());
	}
}

void Parser::checkLocationDirectiveCount()
{
	std::stringstream ss;
	std::vector<std::string> directives;
	directives.push_back("root");
	directives.push_back("index");
	directives.push_back("client_max_body_size");
	directives.push_back("autoindex");
	directives.push_back("limit_except");
	directives.push_back("return");

	// check if the 6 directives have no more than one
	for (int i = 0; i < 6; i++)
	{
		int count = _locationDirectiveCount[directives[i]];
		// std::cout << "directive: " << directives[i] << std::endl;
		// std::cout << "count: " << count << std::endl << std::endl;
		if (count > 1) {
			ss << "Error (location block " << _locationBlockNum - 1 << "): The directive " << directives[i] << " can only be used once";
			throw CustomException(ss.str());
		}
	}
}
