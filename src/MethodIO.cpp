/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:12:22 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/23 13:47:40 by nwai-kea         ###   ########.fr       */
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

std::string MethodIO::getMethod(std::stringstream *ss, std::vector<std::string> token)
{
    if (token[2] != "HTTP/1.1\r" )
    {
        *ss << BRED << generateResponse(400) << RESET;
    }
    else
    {
        std::ifstream file;
        setPath(token[1]);
        file.open(getPath(), std::ifstream::in);
        if (file.fail())
        {
            setPath("www/notfound.html");
            file.open(getPath(), std::ifstream::in);
            if (file.fail())
                *ss << BRED << generateResponse(404) << RESET;
            else
                *ss << BRED << generateResponse(404) << WHITE << file.rdbuf() << RESET;
            file.close();
        }
        else
            *ss << BGREEN << generateResponse(200) << WHITE << file.rdbuf() << RESET;
        file.close();
    }
    return (ss->str());
}

std::string MethodIO::delMethod(std::stringstream *ss, std::vector<std::string> token)
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
        if (!file.is_open())
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

std::string MethodIO::postMethod(std::stringstream *ss, std::vector<std::string> token)
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

std::string MethodIO::getMessageToSend()
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
            break;
        case 2:
            return delMethod(&ss, token);
            break;
        default:
            return("Bruh");
            break;
    }
}

std::string MethodIO::getMessage(int code) const
{
    switch (code) {
        case 200:
        {
            return ("OK");
        }
        case 400:
        {
            return ("Bad Request");
        }
        case 404:
        {
            return ("Not Found");
        }
        default:
            return ("Undefined");
    }
}

std::string MethodIO::getDate() const
{
    const std::string day[] = {"Sun","Mon","Tue",
        "Wed","Thu","Fri","Sat"};
    char date[100];

    time_t now = time(0);
    tm *t = localtime(&now);
    std::strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", t);
    std::string dateStr(date);
    return (dateStr);
}

std::string MethodIO::getLen() const
{
    std::string path = getPath();
    std::string size;
    std::ifstream f;

    f.open(path, std::ifstream::in);
    if (f.is_open())
    {
        f.ignore(std::numeric_limits<std::streamsize>::max());
        std::streamsize len = f.gcount();
        f.clear();
        f.seekg(0, std::ios_base::beg);
        int l = static_cast<int>(len);
        f.close();
        std::stringstream str;
        str << l;
        str >> size;
        return ("Content-Length: " + size + "\r\n");
    }
    else
        return ("File not found/Content Error\r\n");
}

std::string MethodIO::getType() const 
{
    std::string p;
    std::string extension;
    size_t file = getPath().find_last_of("/\\");
    if (file != std::string::npos)
        p = getPath().substr(file + 1);
    file = p.rfind(".");
    if (file != std::string::npos)
        extension = p.substr(file + 1);
    if (extension == "html")
        return ("Content-Type: text/html\r\n");
    else if (extension == "css")
        return ("Content-Type: text/css\r\n");
    else if (extension == "txt")
        return ("Content-Type: text/plain\r\n");
    else
        return ("\r\n");    
}

std::string MethodIO::getMap() const
{
    return ("Date: " + getDate() + "\r\nServer: webserv" + 
        "\r\n" + getLen() + getType() + 
        "\r\n");  
}

std::string MethodIO::generateResponse(int code)
{
    std::ostringstream conv;

    setCode(code);
    conv << code;
    this->statusLine = "HTTP/1.1 " + conv.str() + " " + getMessage(code) + "\r\n";
    this->response = getMap(); 
    return (this->statusLine + this->response);
}

void MethodIO::setPath(std::string path)
{
    if (path == "/")
        this->path = "www/index.html";
    else
        this->path = path;
}

void MethodIO::setCode(int code)
{
    this->_code = code;
}

int MethodIO::getCode() const
{
    return (this->_code);
}

std::string MethodIO::getPath() const
{
    return this->path;
}