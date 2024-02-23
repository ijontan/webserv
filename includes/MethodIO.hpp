/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:11 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/23 22:46:03 by nwai-kea         ###   ########.fr       */
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
    int _code;
    std::string statusLine;
    // std::map<std::string, std::string> responseHeader;
    std::string response;
    std::string path;

    void setPath(std::string path);
    void setCode(int code);

public:
    MethodIO(void);
    ~MethodIO(void);
    MethodIO(const MethodIO &src);
    MethodIO &operator=(const MethodIO &rhs);
    std::string generateResponse(int code);
	std::string getMessageToSend();
    std::string getMethod(std::stringstream *ss, std::vector<std::string> token);
    std::string postMethod(std::stringstream *ss, std::vector<std::string> token);
    std::string delMethod(std::stringstream *ss, std::vector<std::string> token);
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