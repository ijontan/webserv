
#include "webserv.hpp"
#include "Socket.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./webserv <port>" << std::endl;
        return 1;
    }
    Socket sock(argv[1]);
    sock.initSocket();
    sock.acceptLoop();
    return 0;
}
