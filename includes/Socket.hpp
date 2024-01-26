#pragma once

#include <string>
#include <iostream>
#include "webserv.hpp"
#include <string.h>
#include <unistd.h>

class Socket
{
private:
    int sockfd;
    std::string port;

public:
    Socket(void);
    Socket(const std::string &port);
    ~Socket(void);
    Socket(const Socket &src);
    Socket &operator=(const Socket &rhs);
    void initSocket(void);
    void acceptLoop(void);
};