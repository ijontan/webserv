/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:42:11 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/20 14:33:31 by nwai-kea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IOAdaptor.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "colors.h"

#define GET 0
#define POST 1
#define DELETE 2

class MethodIO : public IOAdaptor
{
private:
public:
    MethodIO(void);
    ~MethodIO(void);
    MethodIO(const MethodIO &src);
    MethodIO &operator=(const MethodIO &rhs);
	std::string getMessageToSend() const;
    std::string getMethod(std::stringstream *ss, std::vector<std::string> token) const;
    std::string postMethod(std::stringstream *ss, std::vector<std::string> token) const;
    std::string delMethod(std::stringstream *ss, std::vector<std::string> token) const;
    int chooseMethod(std::vector<std::string> token) const;
};