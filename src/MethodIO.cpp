/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:12:22 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/03/01 18:14:52 by nwai-kea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodIO.hpp"
#include "utils.hpp"
#include <cstddef>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
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
	m[400] = "Bad Request";
	m[403] = "Forbidden Error";
	m[404] = "Not Found";
	m[409] = "Conflict";
	return m;
}

std::map<std::string, std::string> MethodIO::initContentTypes()
{
	std::map<std::string, std::string> m;
	m["txt"] = "text/plain";
	m["html"] = "text/html";
	m["css"] = "text/css";
	m["js"] = "text/javascript";

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

std::string MethodIO::getMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	int code = 200;
	if (rqi.request[2] != "HTTP/1.1")
		return generateResponse(400, rsi);
	std::string path = getPath(rqi.request[1], ws);
	std::ifstream file(path.c_str());
	if (file.fail())
	{
		code = 404;
		file.close();
		path = getPath("/notfound.html", ws);
		file.open(path.c_str());
		if (file.fail())
			return generateResponse(code, rsi);
	}
	rsi.headers["Content-Length"] = getLen(file);
	rsi.headers["Content-Type"] = getType(path);
	std::ostringstream body;
	body << file.rdbuf();
	rsi.body.append(body.str());
	file.close();
	return (generateResponse(code, rsi));
}

std::string MethodIO::headMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	int code = 200;
	if (rqi.request[2] != "HTTP/1.1")
		return generateResponse(400, rsi);
	std::string path = getPath(rqi.request[1], ws);
	std::ifstream file(path.c_str());
	if (file.fail())
	{
		code = 404;
		file.close();
		path = getPath("/notfound.html", ws);
		file.open(path.c_str());
		if (file.fail())
			return generateResponse(code, rsi);
	}
	rsi.headers["Content-Type"] = getType(path);
	file.close();
	return (generateResponse(code, rsi));
}

std::string MethodIO::delMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	std::ifstream file;
	std::ifstream file2;
	std::stringstream ss;

	if (rqi.request[1] != "HTTP/1.1\r")
		return generateResponse(400, rsi);
	std::string path = getPath(rqi.request[1], ws);
	file.open(path.c_str(), std::ifstream::in);
	if (file.fail())
	{
		if (access(getPath(rqi.request[1], ws).c_str(), W_OK) != 0)
			return generateResponse(403, rsi);
		file2.open(getPath("/notfound.html", ws).c_str(), std::ifstream::in);
		if (file2.fail())
			ss << generateResponse(404, rsi);
		else
		{
			std::ostringstream body;
			body << file.rdbuf();
			rsi.body.append(body.str());
			ss << generateResponse(404, rsi) << rsi.body;
		}
		file2.close();
	}
	else
	{
		if (std::remove(path.c_str()) == 0)
			ss << generateResponse(204, rsi);
		else
			ss << generateResponse(403, rsi);
	}
	file.close();
	return (ss.str());
}

std::string MethodIO::postMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	std::ifstream file;
	std::stringstream ss;

	if (rqi.request[2] != "HTTP/1.1")
		return generateResponse(400, rsi);
	file.open(getPath(rqi.request[1], ws).c_str(), std::ifstream::in);
	if (file.fail())
		ss << generateResponse(204, rsi);
	else
	{
		std::ofstream output(getPath(rqi.request[1], ws).c_str());

		if (output.fail())
			ss << generateResponse(403, rsi);
		else
			ss << generateResponse(204, rsi);
		output.close();
	}
	file.close();
	return (ss.str());
}

std::string MethodIO::putMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi)
{
	std::ifstream file;
	std::stringstream ss;

	if (rqi.request[2] != "HTTP/1.1")
		return generateResponse(400, rsi);
	file.open(getPath(rqi.request[1], ws).c_str(), std::ifstream::in);
	if (file.fail())
		ss << generateResponse(403, rsi);
	else
		ss << generateResponse(201, rsi);
	file.close();
	return (ss.str());
}

std::string MethodIO::getMessageToSend(WebServer &ws)
{
	std::string word;
	MethodIO::rInfo rqi;
	MethodIO::rInfo rsi;

	tokenize(getRaw(), rqi);
	std::string method = rqi.request[0];
	rsi.headers["Date"] = getDate();
	std::map<std::string, MethodPointer>::const_iterator it = methods.find(method);
	if (it != methods.end())
		return (it->second)(ws, rqi, rsi);
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

std::string MethodIO::getLen(std::ifstream &file)
{
	std::stringstream ss;

	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	ss << fileSize;
	return ss.str();
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
		return "";
	return it->second;
}

std::string MethodIO::generateResponse(int code, MethodIO::rInfo &rsi)
{
	std::ostringstream ss;
	std::map<std::string, std::string>::iterator it;

	ss << "HTTP/1.1 " << code << " " << getMessage(code) << "\r\n";
	for (it = rsi.headers.begin(); it != rsi.headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	ss << "\r\n" << rsi.body;
	return (ss.str());
}

std::string MethodIO::getPath(std::string basePath, WebServer &ws)
{
	(void)ws;
	return basePath == "/" ? "www/index.html" : "www" + basePath;
}

void MethodIO::tokenize(std::string s, MethodIO::rInfo &rsi) const
{
	std::pair<std::string, std::string> headerBody = utils::splitPair(s, "\r\n\n");
	std::vector<std::string> requestHeader = utils::split(headerBody.first, "\r\n");

	rsi.request = utils::split(requestHeader[0], ' ');
	for (size_t i = 1; i < requestHeader.size(); i++)
		rsi.headers.insert(utils::splitPair(requestHeader[i], ": "));
	rsi.body = headerBody.second;
}
