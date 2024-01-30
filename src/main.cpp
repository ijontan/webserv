
#include "webserv.h"
#include "Socket.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./webserv <port>" << std::endl;
        return 1;
    }
    NetIO io;
    Socket sock(argv[1]);
    sock.initSocket();
    sock.acceptLoop(io);
    return 0;
}
