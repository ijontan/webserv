
#include "webserv.h"
#include "Socket.hpp"

int main(int argc, char const *argv[])
{
    NetIO io;
    if (argc == 1)
    {
        Server server;
        server.loop(io);
    }
    else if (argc == 2)
    {
        Server server(argv[1]);
        server.loop(io);
    }
    else
    {
        Server server(&argv[1]);
        server.loop(io);
    }
    return 0;
}
