
#include "webserv.hpp"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *servInfo, *p;
    int sockFd, newFd;

    if (argc != 2)
    {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, argv[1], &hints, &servInfo) != 0)
    {
        std::cerr << "getaddrinfo error" << std::endl;
        return 2;
    }
    for (p = servInfo; p != NULL; p = p->ai_next)
    {
        if ((sockFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "socket error" << std::endl;
            continue;
        }

        if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &sockFd, sizeof(int)) == -1)
        {
            std::cerr << "Error setting socket options" << std::endl;
            close(sockFd); // Don't forget to close the socket in case of an error
            return 1;
        }

        if (bind(sockFd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockFd);
            std::cerr << "bind error" << std::endl;
            continue;
        }
        break;
    }
    freeaddrinfo(servInfo); // free the linked list
    while (1)
    {
        if (listen(sockFd, 10))
        {
            std::cerr << "listen error" << std::endl;
            return 4;
        }
        addr_size = sizeof(their_addr);
        newFd = accept(sockFd, (struct sockaddr *)&their_addr, &addr_size);

        std::string str = "";
        char buff[20];
        while (recv(newFd, buff, 20, 0) > 0)
        {
            str += buff;
            memset(buff, 0, 20);
        }
        std::cout << "got message from " << newFd << ": " << str << std::endl;
        close(newFd);
    }
    return 0;
}
