/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:11 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/26 18:52:20 by nwai-kea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IOAdaptor.hpp"

#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <limits>
#include <unistd.h>
#include "colors.h"

#define GET 0
#define POST 1
#define DELETE 2

class MethodIO : public IOAdaptor
{
private:
    struct requestInfo
	{
		std::vector<std::string> request;
		std::map<std::string, std::string> headers;
		std::string body;
	};
    struct responseInfo
	{
		std::vector<std::string> response;
		std::map<std::string, std::string> headers;
        std::string body;
	};
    int _code;
    std::string statusLine;
    // std::map<std::string, std::string> responseHeader;
    std::string response;
    std::string path;

    void setPath(std::string path);
    void setCode(int code);
    void tokenize(std::string s, MethodIO::requestInfo &ri) const;

public:
    MethodIO(void);
    ~MethodIO(void);
    MethodIO(const MethodIO &src);
    MethodIO &operator=(const MethodIO &rhs);
    std::string generateResponse(int code, MethodIO::responseInfo &ri);
	std::string getMessageToSend();
    std::string getMethod(std::stringstream *ss, std::vector<std::string> token, MethodIO::responseInfo &ri);
    std::string postMethod(std::stringstream *ss, std::vector<std::string> token, MethodIO::responseInfo &ri);
    std::string headMethod(std::stringstream *ss, std::vector<std::string> token, MethodIO::responseInfo &ri);
    std::string delMethod(std::stringstream *ss, std::vector<std::string> token, MethodIO::responseInfo &ri);
    std::string putMethod(std::stringstream *ss, std::vector<std::string> token, MethodIO::responseInfo &ri);
    int getCode() const;
    std::string getMap() const;
    std::string getDate() const;
    std::string getPath() const;
    std::string getLen() const;
    std::string getType() const;
    int chooseMethod(std::vector<std::string> token) const;

    // void setCode(int code) const;
    std::string getMessage(int code) const;
};