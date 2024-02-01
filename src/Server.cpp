#include "Server.hpp"

Server::Server(void)
{
}

Server::Server(char const *port)
{
    newSocket(port);
}

Server::Server(char const **ports)
{
    for (size_t i = 0; ports[i] != 0; i++)
    {
        newSocket(ports[i]);
    }
}

Server::Server(const Server &src)
{
    *this = src;
}

Server &Server::operator=(const Server &rhs)
{
    (void)rhs;
    return *this;
}

Server::~Server(void)
{
}

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void Server::loop(IOAdaptor io)
{
    char s[INET6_ADDRSTRLEN];
    int newFd;
    struct sockaddr_storage theiraddr;
    socklen_t addrSize = sizeof(theiraddr);
    char buff[256];
    std::vector<std::string> str;

    str.push_back("");
    for (size_t j = 0; j < sockets.size(); j++)
        std::cout << sockets[j];

    for (;;)
    {
        int pollCount = poll(&pfds[0], pfds.size(), -1);
        if (pollCount == -1)
        {
            std::cerr << "poll error" << std::endl;
            return;
        }

        for (size_t i = 0; i < pfds.size(); i++)
        {
            if (!(pfds[i].revents & POLLIN))
                continue;

            // find if socket exist
            bool found = false;
            for (size_t j = 0; j < sockets.size(); j++)
                if (sockets[j].getSockfd() == pfds[i].fd)
                    found = true;

            if (found)
            {
                addrSize = sizeof(theiraddr);
                newFd = accept(pfds[i].fd, (struct sockaddr *)&theiraddr, &addrSize);
                if (newFd == -1)
                {
                    std::cerr << "accept error" << std::endl;
                    continue;
                }
                inet_ntop(theiraddr.ss_family,
                          get_in_addr((struct sockaddr *)&theiraddr),
                          s, sizeof(s));
                std::cout << HGREEN << "Server: got connection from: " << RESET << s << std::endl;
                str.push_back("");
            }
            else
            {
                int bytes = recv(pfds[i].fd, buff, sizeof(buff), 0);
                if (bytes <= 0)
                {
                    io.recieveMessage(str[i]);
                    if (bytes == 0)
                    {
                        std::cout << io;
                        std::string toSend = io.getMessageToSend();
                        send(newFd, toSend.c_str(), toSend.length(), 0);
                    }
                    else
                    {
                        std::cerr << "recv error" << std::endl;
                    }
                    str.erase(str.begin() + i);
                    close(pfds[i].fd);
                    io.recieveMessage("");
                }
                else
                {
                    str[i] += buff;
                    memset(buff, 0, sizeof(buff));
                }
            }
        }
    }
}

void Server::addPfd(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pfds.push_back(pfd);
}

void Server::removePfd(int index)
{
    pfds.erase(pfds.begin() + index);
}

void Server::newSocket(char const *port)
{

    Socket s = Socket(port);
    s.initSocket();
    std::cout << "socket port:" << port << ", fd:" << s;
    addPfd(s.getSockfd());
    sockets.push_back(s);
}