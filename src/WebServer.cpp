
#include "webserv.h"

WebServer::WebServer(const std::string &filePath)
{
	try
	{
		Parser parser(filePath);

		parser.parseServerBlocks(this->_serverBlocks);
		std::cout << GREEN "Server blocks created" RESET << std::endl
				  << std::endl;

		printServerBlocksInfo();
		initSockets();
	}
	catch (const std::exception &e)
	{
		std::cout << BRED << e.what() << RESET << std::endl;
	}
}

WebServer::~WebServer() {}

WebServer::WebServer(const WebServer &other)
{
	(void)other;
}

WebServer &WebServer::operator=(const WebServer &other)
{
	(void)other;
	return *this;
}

void WebServer::printServerBlocksInfo()
{
	for (std::vector<ServerBlock>::iterator it = this->_serverBlocks.begin(); it != this->_serverBlocks.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}

void WebServer::initSockets()
{
	for (std::vector<ServerBlock>::iterator it = _serverBlocks.begin(); it < _serverBlocks.end(); it++) {
		(*it).initSocket();
		addPfd((*it).getSockfd());
	}
}

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void WebServer::loop(IOAdaptor io)
{
    char s[INET6_ADDRSTRLEN];
    int newFd;
    struct sockaddr_storage theiraddr;
    socklen_t addrSize = sizeof(theiraddr);
    char buff[256];
    std::vector<std::string> str;

    str.push_back("");
    for (size_t j = 0; j < _serverBlocks.size(); j++)
        std::cout << _serverBlocks[j];

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
            for (size_t j = 0; j < _serverBlocks.size(); j++)
                if (_serverBlocks[j].getSockfd() == pfds[i].fd)
                    found = true;
            std::cout << "found: " << found << std::endl;

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
                str[i] = "";
                addPfd(newFd);
            }
            else
            {
                std::cout << "recieved message:" << std::endl;
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
                    removePfd(i);
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

void WebServer::addPfd(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pfds.push_back(pfd);
}

void WebServer::removePfd(int index)
{
    pfds.erase(pfds.begin() + index);
}

