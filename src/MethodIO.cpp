#include "MethodIO.hpp"
#include "ABlock.hpp"
#include "AutoIndex.hpp"
#include "LocationBlock.hpp"
#include "Cgi.hpp"
#include "RequestException.hpp"
#include "ServerBlock.hpp"
#include "LocationBlock.hpp"
#include "WebServer.hpp"
#include "colors.h"
#include "utils.hpp"
#include <cstddef>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

const std::map<std::string, MethodIO::MethodPointer> MethodIO::methods = initMethodsMap();
const std::map<int, std::string> MethodIO::errCodeMessages = initErrCodeMessages();
const std::map<std::string, std::string> MethodIO::contentTypes = initContentTypes();

std::map<std::string, MethodIO::MethodPointer> MethodIO::initMethodsMap()
{
	std::map<std::string, MethodIO::MethodPointer> m;
	m["GET"] = &MethodIO::getMethod;
	m["POST"] = &MethodIO::postMethod;
	m["HEAD"] = &MethodIO::headMethod;
	m["DELETE"] = &MethodIO::delMethod;
	m["PUT"] = &MethodIO::putMethod;
	return m;
}

std::map<int, std::string> MethodIO::initErrCodeMessages()
{
	std::map<int, std::string> m;
	m[200] = "OK";
	m[201] = "Created";
	m[204] = "No Content";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[303] = "See Other";
	m[400] = "Bad Request";
	m[403] = "Forbidden Error";
	m[404] = "Not Found";
	m[408] = "Request Timeout";
	m[409] = "Conflict";
	m[413] = "Content Too Large";
	m[415] = "Unsupported Media Type";
	m[500] = "Internal Server Error";
	return m;
}

std::map<std::string, std::string> MethodIO::initContentTypes()
{
	std::map<std::string, std::string> m;
	m["txt"] = "text/plain";
	m["html"] = "text/html";
	m["css"] = "text/css";
	m["js"] = "text/javascript";
	m["png"] = "image/png";
	m["ico"] = "image/ico";
	m["cpp"] = "text/cpp";

	return m;
}

MethodIO::MethodIO(void) : IOAdaptor()
{
}

MethodIO::MethodIO(const MethodIO &src) : IOAdaptor()
{
	*this = src;
}

MethodIO &MethodIO::operator=(const MethodIO &rhs)
{
	(void)rhs;
	return *this;
}

MethodIO::~MethodIO(void)
{
}

std::string MethodIO::getMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	rsi.body = readFile(rqi, rsi, block);
	rsi.headers["Content-Length"] = utils::to_string(rsi.body.size());
	std::string ext = rqi.path.substr(rqi.path.find_last_of(".") + 1);
	if (rqi.exist == true && (ext == "py" || ext == "cgi"))
		return rsi.body;
	return (generateResponse(rsi.code, rsi));
}

std::string MethodIO::headMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	std::string body = readFile(rqi, rsi, block);
	rsi.headers["Content-Type"] = getType(rqi.path);
	rsi.headers["Content-Length"] = utils::to_string(body.size());
	return (generateResponse(200, rsi));
}

std::string MethodIO::delMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	writeFile(rqi, block, false);
	if (std::remove(rqi.path.c_str()))
		throw RequestException("Cannot Delete File", 403);
	return generateResponse(204, rsi);
}

std::string MethodIO::postMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	writeFile(rqi, block, true);
	if (rqi.exist == true)
	{
		std::ostringstream oss;
		size_t dirPos = rqi.path.find_first_of("/");
		std::string ext = rqi.path.substr(rqi.path.find_last_of(".") + 1);
		std::string input;
		rsi.headers["Content-Type"] = getType(rqi.path);
		rsi.headers["Content-Length"] = utils::to_string(rqi.query.size());
		// std::cout << "SIZE: " << rsi.headers["Content-Length"] << std::endl;
		if ((dirPos != std::string::npos) && (ext == "py"))
		{
			// if (rsi.headers["Content-Type"] != "application/x-www-form-urlencoded" && rsi.headers["Content-Type"] != "multipart/form-data" && rsi.headers["Content-Type"] != "text/plain")
			// 	return generateResponse(415, rsi);
			Cgi cgi(rqi.request, rqi.headers, rqi.path, rqi.body, rqi.query);
			if (cgi.runCgi() == 200)
			{
				rqi.exist = true;
				rsi.body = cgi.getBody();
				return rsi.body;
			}
			else
				throw RequestException("Internal Server Error", 500);
		}
		else {
			return generateResponse(409, rsi);
		}
	}
	return generateResponse(200, rsi);
}

std::string MethodIO::putMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	writeFile(rqi, block, false);
	rsi.headers["Content-Type"] = getType(rqi.path);
	rsi.headers["Content-Length"] = utils::to_string(rqi.body.size());
	return generateResponse(204, rsi);
}

std::string MethodIO::getMessageToSend(WebServer &ws, std::string port)
{
	MethodIO::rInfo requestInfo;
	MethodIO::rInfo responseInfo;
	ServerBlock block;

	try
	{
		tokenize(getRaw(), requestInfo);
		if (requestInfo.request[2] != "HTTP/1.1")
			return generateResponse(400, responseInfo);
		requestInfo.port = port;
		block = getServerBlock(requestInfo, ws);
		if (block.getClientMaxBodySize() < (int)requestInfo.body.size() && block.getClientMaxBodySize() != 0)
			throw RequestException("Content Too Large", 413);
		// std::cout << getRaw() << requestInfo.request[1] << std::endl;
		// std::string path = getPath(requestInfo.request[1], ws, port);
		// requestInfo.path = path;
		// std::string test = path.substr(path.find_last_of(".") + 1);

		// check if extension is python (if possible change this so it detects if script is from cgi folder)
		// size_t dirPos = requestInfo.queryPath.find_first_of("/");
		// std::string ext = requestInfo.queryPath.substr(requestInfo.queryPath.find_last_of(".") + 1);
		// if ((dirPos != std::string::npos) && (ext == "py" || ext == "cgi"))
		// {
		// 	Cgi cgi(requestInfo.request, requestInfo.headers, requestInfo.queryPath, requestInfo.body, requestInfo.query);
		// 	if (cgi.runCgi() == 200)
		// 	{
		// 		std::cout << cgi.getBody() << std::endl;
		// 		responseInfo.body.append(cgi.getBody());
		// 	}
		// 	else
		// 		generateResponse(500, responseInfo);

		// }
		// else
		// 	generateResponse(500, responseInfo);
		std::string method = requestInfo.request[0];
		responseInfo.headers["Date"] = getDate();
		std::map<std::string, MethodPointer>::const_iterator it = methods.find(method);
		if (it != methods.end())
			return (it->second)(block, requestInfo, responseInfo);
	}
	catch (RequestException &e)
	{
		int code = e.getCode();
		std::cerr << BRED << "Error: " << e.what() << std::endl
				  << "Error Code: " << code << " " << errCodeMessages.find(code)->second << RESET << std::endl;
		if (block.getRootDirectory() == "")
			return generateResponse(code, responseInfo);
		std::string path = block.getRootDirectory() + "/" + block.getErrorPages()[code];
		std::ifstream file(path.c_str());
		std::ostringstream oss;
		oss << file.rdbuf();
		responseInfo.body = oss.str();
		responseInfo.headers["Content-Type"] = getType(path);
		responseInfo.headers["Content-Length"] = utils::to_string(responseInfo.body.size());
		return generateResponse(code, responseInfo);
	}
	std::cerr << "methods not found" << std::endl;
	return "bruh";
}

std::string MethodIO::getMessage(int code)
{
	std::map<int, std::string>::const_iterator val = errCodeMessages.find(code);
	if (val != errCodeMessages.end())
		return val->second;
	return "Undefined";
}

std::string MethodIO::getDate()
{
	char date[100];

	time_t now = time(0);
	tm *t = localtime(&now);
	std::strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", t);
	std::string dateStr(date);
	return (dateStr);
}

std::string MethodIO::getType(std::string path)
{
	std::string p;
	std::vector<std::string> tokens = utils::split(path, '.');
	std::string extension = tokens[tokens.size() - 1];

	if (tokens.size() == 1)
		extension = "txt";
	std::map<std::string, std::string>::const_iterator it = contentTypes.find(extension);
	if (it == contentTypes.end())
		return "text/plain";
	return it->second;
}

std::string MethodIO::generateResponse(int code, MethodIO::rInfo &rsi)
{
	std::ostringstream ss;
	std::map<std::string, std::string>::iterator it;

	ss << "HTTP/1.1 " << code << " " << getMessage(code) << "\r\n";
	for (it = rsi.headers.begin(); it != rsi.headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	// if (rsi.request[1] == "GET")
	ss << "\r\n" << rsi.body;
	// std::cout << "RESULT:  " << ss.str() << std::endl;
	return (ss.str());
}

// std::string MethodIO::getPath(std::string basePath, WebServer &ws, std::string &port)
// {
// 	std::vector<ServerBlock> servers = ws.getServers();
// 	std::string root = "www";
//
// 	for (std::vector<ServerBlock>::iterator it = servers.begin(); it != servers.end(); it++)
// 	{
// 		std::vector<std::string> ports = it->getPortsListeningOn();
// 		bool found = false;
// 		for (size_t i = 0; i < ports.size(); i++)
// 		{
// 			if (ports[i] == port)
// 				found = true;
// 		}
// 		if (!found)
// 			continue;
// 		root = it->getRootDirectory();
// 		break;
// 	}
// 	// std::cout << root << std::endl;
// 	return root + "/" + (basePath == "/" ? "index.html" : basePath);
// }

ServerBlock MethodIO::getServerBlock(MethodIO::rInfo &rqi, WebServer &ws)
{
	std::vector<ServerBlock> servers = ws.getServers();
	std::string host = utils::splitPair(rqi.headers["Host"], ":").first;
	for (std::vector<ServerBlock>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		if (utils::find(it->getPortsListeningOn(), rqi.port) && utils::find(it->getServerName(), host))
			return *it;
	}
	throw RequestException("Location not defined in config", 404);
}

std::string MethodIO::readFile(MethodIO::rInfo &rqi, MethodIO::rInfo &rsi, ServerBlock &block)
{
	// try all indexes in the config
	std::pair<std::string, ABlock> blockPair = block.getLocationBlockPair(rqi.queryPath);
	std::vector<std::string> index = blockPair.second.getIndex();
	std::string root = blockPair.second.getRootDirectory();
	std::ifstream file;
	std::string path = root + "/" + utils::splitPair(rqi.queryPath, blockPair.first).second;
	std::cout << "block path: " << blockPair.first << std::endl;
	size_t i;

	rsi.code = 200;
    std::pair<int, std::string> redir = blockPair.second.getRedirection();
    std::cout << "code: " << redir.first << ", path:" << redir.second << std::endl;
	if (redir.first)
	{
		rsi.headers["Location"] = redir.second;
		rsi.code = redir.first;
		return "";
	}
	else if (rqi.queryPath.at(rqi.queryPath.length() - 1) == '/' && rqi.queryPath.length() > 1)
	{
		AutoIndex indexes(path, rqi.queryPath);
		rsi.headers["Content-Type"] = "text/html";
		return indexes.getBody();
	}
	else if (rqi.queryPath != blockPair.first)
	{
		rqi.path = path;
		std::cout << "path: " << path << std::endl;
		if (access(path.c_str(), F_OK))
			throw RequestException("File doesn't exist", 404);
		if (access(path.c_str(), R_OK))
			throw RequestException("File read forbidden", 403);
		// size_t dirPos = rqi.path.find_first_of("/");
		std::string ext = rqi.path.substr(rqi.path.find_last_of(".") + 1);
		if (access(path.c_str(), R_OK) == 0 && (ext == "py" || ext == "cgi"))
		{
			Cgi cgi(rqi.request, rqi.headers, rqi.path, rqi.body, rqi.query);
			if (cgi.runCgi() == 200)
			{
				rqi.exist = true;
				return(cgi.getBody());
			}
			else
				throw RequestException("Internal Server Error", 500);
		}
		file.open(path.c_str());
	}
	if (!file.is_open())
	{
		file.close();
		for (i = 0; i < index.size(); i++)
		{
			std::stringstream ss;
			ss << root << "/" << index[i];
			rqi.path = ss.str();
			if (!access(ss.str().c_str(), F_OK) && access(ss.str().c_str(), R_OK))
				throw RequestException("File read forbidden", 403);
			file.open(ss.str().c_str());
			if (!file.fail())
				break;
			file.close();
		}
		if (i == index.size())
			throw RequestException("File doesn't exist", 404);
	}
	std::ostringstream oss;

	rsi.headers["Content-Type"] = getType(rqi.path);
	// NOTE: Tmp fix for cgi issue, please remove this code if new solution is found!
	// size_t dirPos = rqi.path.find_first_of("/");
	// std::string ext = rqi.path.substr(rqi.path.find_last_of(".") + 1);
	// if ((dirPos != std::string::npos) && (ext == "py" || ext == "cgi"))
	// {
	// 	Cgi cgi(rqi.request, rqi.headers, rqi.path, rqi.body, rqi.query);
	// 	if (cgi.runCgi() == 200)
	// 	{
	// 		rqi.exist = true;
	// 		return(cgi.getBody());
	// 	}
	// 	else
	// 		throw RequestException("Internal Server Error", 500);
	// }
	// else
	oss << file.rdbuf();
	// NOTE: Tmp fix for cgi issue, please remove this code if new solution is found!

	return oss.str();
}

void MethodIO::writeFile(MethodIO::rInfo &rqi, ServerBlock &block, bool createNew)
{
	// try all indexes in the config
	std::pair<std::string, ABlock> blockPair = block.getLocationBlockPair(rqi.queryPath);
	std::vector<std::string> index = blockPair.second.getIndex();
	std::string root = blockPair.second.getRootDirectory();
	std::ofstream file;
	size_t i;
	std::stringstream ss;
	rqi.exist = false;
	if (rqi.request[1] != blockPair.first)
	{
		std::pair<std::string, std::string> pair = utils::splitPair(rqi.queryPath, blockPair.first);

		ss << "./" << root << "/" << pair.second;
		rqi.path = ss.str();
		if (access(ss.str().c_str(), F_OK) && !createNew)
			throw RequestException("File doesn't exist", 404);
		if (access(ss.str().c_str(), W_OK))
			throw RequestException("File write forbidden", 403);
		if (access(ss.str().c_str(), F_OK) == 0 && createNew)
		{
			rqi.exist = true;
			return;
		}
		file.open(ss.str().c_str());
		ss.clear();
	}
	if (!file.is_open() && false)
	{
		file.close();
		for (i = 0; i < index.size(); i++)
		{
			std::stringstream ss;
			ss << root << "/" << index[i];
			rqi.path = ss.str();
			if (access(ss.str().c_str(), F_OK) && !createNew)
				continue;
			if (!access(ss.str().c_str(), F_OK) && access(ss.str().c_str(), W_OK))
				throw RequestException("File write forbidden", 403);
			file.open(ss.str().c_str());
			if (!file.fail())
				break;
			file.close();
		}
		if (i == index.size() && !createNew)
			throw RequestException("File doesn't exist", 404);
	}
	// try relative path from request
	if (!file.is_open())
		throw RequestException("Failed to create file.", 403);

	file << rqi.body;
}

void MethodIO::tokenize(std::string s, MethodIO::rInfo &rsi) const
{
	std::pair<std::string, std::string> headerBody = utils::splitPair(s, "\r\n\n");
	std::vector<std::string> requestHeader = utils::split(headerBody.first, "\r\n");

	rsi.request = utils::split(requestHeader[0], ' ');
	for (size_t i = 1; i < requestHeader.size(); i++)
		rsi.headers.insert(utils::splitPair(requestHeader[i], ": "));
	if (!rsi.request.size())
		throw RequestException("Bad Request (Empty)", 400);
	if (rsi.request[0] == "GET")
	{
		size_t q = rsi.request[1].find_first_of("?");
		if (q != std::string::npos)
		{
			rsi.query = rsi.request[1].substr(q + 1);
			rsi.queryPath = rsi.request[1].substr(0, q);
		}
		else 
			rsi.queryPath = rsi.request[1];
	}
	else if (rsi.request[0] == "POST")
	{
		std::vector<std::string> queryBody = utils::split(s, "\r\n");
		rsi.body = headerBody.first;
		rsi.query = queryBody.back();
		rsi.queryPath = rsi.request[1];
	}
	else 
		rsi.queryPath = rsi.request[1];
	
	rsi.body = headerBody.second;
	// std::pair<std::string, std::string> pair = utils::splitPair(rsi.request[1], "?");
	// rsi.query = pair.second;
	// rsi.queryPath = pair.first;
	// std::cout << rsi.queryPath << std::endl;
}
