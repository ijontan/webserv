#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include "IOAdaptor.hpp"

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
    int getSockfd() const;
};

std::ostream &operator<<(std::ostream &os, const Socket &socket);