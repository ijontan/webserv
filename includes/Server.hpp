#pragma once
#include <string>
#include <iostream>
#include "Socket.hpp"
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include "IOAdaptor.hpp"
#include "colors.h"
#include <arpa/inet.h>
#include <fcntl.h>

class Server
{
private:
    std::vector<Socket> sockets;
    std::vector<struct pollfd> pfds;
    void addPfd(int fd);
    void removePfd(int index);
    void newSocket(char const *port);

public:
    Server(void);
    Server(char const *port);
    Server(char const **ports);
    ~Server(void);
    Server(const Server &src);
    Server &operator=(const Server &rhs);
    void loop(IOAdaptor io);
};