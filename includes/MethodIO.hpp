/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:11 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/03/07 21:44:19 by itan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IOAdaptor.hpp"
#include "Cgi.hpp"

#include <fstream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

#define GET 0
#define POST 1
#define DELETE 2

class MethodIO;
class MethodIO : public IOAdaptor
{
private:
	struct rInfo
	{
		std::vector<std::string> request;
		std::map<std::string, std::string> headers;
		std::string body;
	};
	std::string statusLine;
	typedef std::string (*MethodPointer)(WebServer &, struct rInfo &, struct rInfo &);
	// std::map<std::string, std::string> responseHeader;
	std::string response;
	static const std::map<int, std::string> errCodeMessages;
		static const std::map<std::string, MethodPointer> methods;
		static const std::map<std::string, std::string> contentTypes;

	void tokenize(std::string s, MethodIO::rInfo &ri) const;

	static std::map<std::string, MethodPointer> initMethodsMap();
	static std::map<int, std::string> initErrCodeMessages();
	static std::map<std::string, std::string> initContentTypes();
	static std::string getMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string postMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string headMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string delMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string putMethod(WebServer &ws, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);

	static std::string getDate();
	static std::string getLen(std::ifstream &file);
	static std::string getType(std::string path);
	static std::string getPath(std::string basePath, WebServer &ws);

	// void setCode(int code) const;
	static std::string getMessage(int code);

	std::string getUpdatedContent(int fd);

public:
	MethodIO(void);
	~MethodIO(void);
	MethodIO(const MethodIO &src);
	MethodIO &operator=(const MethodIO &rhs);
	static std::string generateResponse(int code, MethodIO::rInfo &ri);
	std::string getMessageToSend(WebServer &ws, std::string port);
};
