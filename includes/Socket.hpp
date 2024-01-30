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
    struct pollfd fds[1024];
    int pfdCount;

public:
    Socket(void);
    Socket(const std::string &port);
    ~Socket(void);
    Socket(const Socket &src);
    Socket &operator=(const Socket &rhs);
    void initSocket(void);
    void acceptLoop(IOAdaptor &nio);
    void addPfd(int fd);
    void removePfd(int index);
};