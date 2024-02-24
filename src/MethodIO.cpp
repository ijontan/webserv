/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwai-kea <nwai-kea@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:12:22 by nwai-kea          #+#    #+#             */
/*   Updated: 2024/02/24 19:47:13 by nwai-kea         ###   ########.fr       */
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
    else if (token[0] == "HEAD")
        return 3;
    else if (token[0] == "PUT")
        return 4;
    else
        return -1;
}

std::string MethodIO::getMethod(std::stringstream *ss, std::vector<std::string> token)
{
    if (token[2] != "HTTP/1.1" )
        *ss << BRED << generateResponse(400) << RESET;
    else
    {
        std::ifstream file;
        setPath(token[1]);
        file.open(getPath().c_str(), std::ifstream::in);
        if (file.fail())
        {
            setPath("www/notfound.html");
            file.open(getPath().c_str(), std::ifstream::in);
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

std::string MethodIO::headMethod(std::stringstream *ss, std::vector<std::string> token)
{
    if (token[2] != "HTTP/1.1" )
        *ss << BRED << generateResponse(400) << RESET;
    else
    {
        std::ifstream file;
        setPath(token[1]);
        file.open(getPath().c_str(), std::ifstream::in);
        if (file.fail())
        {
            setPath("www/notfound.html");
            file.open(getPath().c_str(), std::ifstream::in);
            if (file.fail())
                *ss << BRED << generateResponse(404) << RESET;
            else
                *ss << BRED << generateResponse(404) << RESET;
            file.close();
        }
        else
            *ss << BGREEN << generateResponse(200) << RESET;
        file.close();
    }
    return (ss->str());
}

std::string MethodIO::delMethod(std::stringstream *ss, std::vector<std::string> token)
{
    std::ifstream file;
    std::ifstream file2;

    if (token[2] != "HTTP/1.1" )
        *ss << BRED << generateResponse(400) << RESET;
    else
    {
        setPath(token[1]);
        file.open(getPath().c_str(), std::ifstream::in);
        if (file.fail())
        {
            if(access(getPath().c_str(), W_OK) != 0)
                *ss << BRED << generateResponse(403) << RESET;
            else
            {
                setPath("www/notfound.html");
                file2.open(getPath().c_str(), std::ifstream::in);
                if (file2.fail())
                    *ss << BRED << generateResponse(404) << RESET;
                else
                    *ss << BRED << generateResponse(404) << WHITE << file.rdbuf() << RESET;
                file2.close();
            }
        }
        else
        {
            file.close();
            if (std::remove(getPath().c_str()) == 0)
            {
                *ss << BGREEN << generateResponse(204) << RESET;
            }
            else
            {
                *ss << BRED << generateResponse(403) << RESET;
            }
        }
    }
    return (ss->str());
}

std::string MethodIO::postMethod(std::stringstream *ss, std::vector<std::string> token)
{
    std::ifstream file;

    if (token[2] != "HTTP/1.1" )
        *ss << BRED << generateResponse(400) << RESET;
    else
    {
        setPath(token[1]);
        file.open(getPath().c_str(), std::ifstream::in);
        if (file)
        {
            file.close();
            *ss << BGREEN << generateResponse(204) << RESET;
        }
        else
        {
            std::ofstream output(getPath().c_str());

            if (output.fail())
            {
                *ss << BRED << generateResponse(403) << RESET;
            }
            else
            {
                output.close();
                *ss << BGREEN << generateResponse(204) << RESET;
            }
            
        }
    }
    return (ss->str());
}

std::string MethodIO::putMethod(std::stringstream *ss, std::vector<std::string> token)
{
    std::ifstream file;

    if (token[2] != "HTTP/1.1" )
        *ss << BRED << generateResponse(400) << RESET;
    else
    {
        setPath(token[1]);
        std::ofstream output(getPath().c_str());
        if (output.fail())
        {
            *ss << BRED << generateResponse(403) << RESET;
        }
        else
        {
            output.close();
            *ss << BGREEN << generateResponse(201) << RESET;
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
            return getMethod(&ss, token);
        case 1:
            return postMethod(&ss, token);
        case 2:
            return delMethod(&ss, token);
        case 3:
            return headMethod(&ss, token);
        case 4:
            return putMethod(&ss, token);
        default:
            return("Bruh");
    }
}

std::string MethodIO::getMessage(int code) const
{
    switch (code) {
        case 200:
        {
            return ("OK");
        }
        case 201:
        {
            return ("Created");
        }
        case 204:
        {
            return ("No Content");
        }
        case 400:
        {
            return ("Bad Request");
        }
        case 403:
        {
            return ("Forbidden Error");
        }
        case 404:
        {
            return ("Not Found");
        }
        case 409:
        {
            return ("Conflict");
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

    f.open(path.c_str(), std::ifstream::in);
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
        return ("\r\n");
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
        this->path = "testing.txt";
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
