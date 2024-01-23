
#include "webserv.hpp"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
        // fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errno));
        return 2;
    }

    sockFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (bind(sockFd, res->ai_addr, res->ai_addrlen))
    {
        printf("bind error\n");
        return 3;
    }

    if (listen(sockFd, 10))
    {
        printf("listen error\n");
        return 4;
    }
    addr_size = sizeof(their_addr);
    newFd = accept(sockFd, (struct sockaddr *)&their_addr, &addr_size);

    char buff[20];
    while (recv(newFd, buff, 20, 0) > 0)
    {
        printf("server: got connection from %s\n", buff);
        send(newFd, "Hello, world!", 13, 0);
    }
    freeaddrinfo(res); // free the linked list
    (void)newFd;
    return 0;
}
