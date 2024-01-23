
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
    struct addrinfo hints, *res;
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

    if (getaddrinfo(NULL, argv[1], &hints, &res) != 0)
    {
        std::cerr << "getaddrinfo error" << std::endl;
        return 2;
    }

    sockFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int reuse = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
    {
        std::cerr << "Error setting socket options" << std::endl;
        close(sockFd); // Don't forget to close the socket in case of an error
        return 1;
    }

    if (bind(sockFd, res->ai_addr, res->ai_addrlen))
    {
        std::cerr << "bind error" << std::endl;
        return 3;
    }
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
    freeaddrinfo(res); // free the linked list
    return 0;
}
