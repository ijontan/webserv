/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:12:22 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/21 00:49:05 by nwai-kea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodIO.hpp"

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

int MethodIO::chooseMethod(std::vector<std::string> token) const
{
    if (token[0] == "GET")
        return 0;
    else if (token[0] == "POST")
        return 1;
    else if (token[0] == "DELETE")
        return 2;
    else
        return -1;
}

std::string MethodIO::getMethod(std::stringstream *ss, std::vector<std::string> token) const
{
    std::ifstream file;

    if (token[2] != "HTTP/1.1\r" )
    {
        *ss << BRED << "HTTP/1.1 400 Bad Request\r\n" 
        << "Content-Type: text/plain\r\n"
        << "\r\n"
        << "400 Bad Request" << RESET;
    }
    else
    {
        std::cout << "Checking file..." << token[1] << std::endl;
        file.open(token[1], std::ifstream::in);
        if (file.fail())
        {
            *ss << BRED << "HTTP/1.1 404 Not Found\r\n" 
            << "Content-Type: text/plain\r\n"
            << "\r\n"
            << "404 Not Found" << RESET;
        }
        else
        {
            *ss << BGREEN << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: text/plain\r\n"
            << "\r\n" << RESET; 
        }
        file.close();
    }
    return (ss->str());
}

std::string MethodIO::delMethod(std::stringstream *ss, std::vector<std::string> token) const
{
    std::ifstream file;

    if (token[2] != "HTTP/1.1\r" )
    {
        *ss << BRED << "HTTP/1.1 400 Bad Request\r\n" 
        << "Content-Type: text/plain\r\n"
        << "\r\n"
        << "400 Bad Request" << RESET;
    }
    else
    {
        std::cout << "Checking file..." << token[1] << std::endl;
        file.open(token[1], std::ifstream::in);
        if (file.fail())
        {
            *ss << BRED << "HTTP/1.1 404 Not Found\r\n" 
            << "Content-Type: text/plain\r\n"
            << "\r\n"
            << "404 Not Found" << RESET;
        }
        else
        {
            file.close();
            if (remove(token[1].c_str()) == 0)
            {
                *ss << BGREEN << "HTTP/1.1 204 No Content\r\n"
                << "Content-Type: text/plain\r\n"
                << "\r\n" << RESET;
            }
            else
            {
                *ss << BRED << "HTTP/1.1 403 No Forbidden Error\r\n"
                << "Content-Type: text/plain\r\n"
                << "\r\n" << RESET;
            }
        }
    }
    return (ss->str());
}

std::string MethodIO::postMethod(std::stringstream *ss, std::vector<std::string> token) const
{
    std::ifstream file;

    if (token[2] != "HTTP/1.1\r" )
    {
        *ss << BRED << "HTTP/1.1 400 Bad Request\r\n" 
        << "Content-Type: text/plain\r\n"
        << "\r\n"
        << "400 Bad Request" << RESET;
    }
    else
    {
        std::cout << "Checking file..." << token[1] << std::endl;
        file.open(token[1], std::ifstream::in);
        if (file)
        {
            *ss << BRED << "HTTP/1.1 409 Conflict\r\n" 
            << "Content-Type: text/plain\r\n"
            << "\r\n"
            << "409 Conflict" << RESET;
        }
        else
        {
            std::ofstream output(token[1]);

            if (output.fail())
            {
                *ss << BRED << "HTTP/1.1 403 Forbidden\r\n" 
                << "Content-Type: text/plain\r\n"
                << "\r\n"
                << "403 Forbidden" << RESET;
            }
            else
            {
                output.close();
                *ss << BGREEN << "HTTP/1.1 204 No Content\r\n"
                << "Content-Type: text/plain\r\n"
                << "\r\n" << RESET;
            }
            
        }
    }
    return (ss->str());
}

std::string MethodIO::getMessageToSend() const
{
    std::stringstream ss;
    std::stringstream str(getRaw());
	std::string word;
    std::vector<std::string> token;

	while (std::getline(str, word))
    {
        std::string tok;
        std::stringstream w(word);
        while (getline(w, tok, ' '))
            token.push_back(tok);
    }
    int method = chooseMethod(token);
    switch(method)
    {
        case 0:
        {
            return getMethod(&ss, token);
            break;
        }
        case 1:
            return postMethod(&ss, token);
        case 2:
            return delMethod(&ss, token);
        default:
            return("Bruh");
    }
}