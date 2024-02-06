
#include "IOAdaptor.hpp"
#include "webserv.h"

int main(int ac, char **av)
{
	IOAdaptor io;
	if (ac == 1)
	{
		WebServer webServer(DEFAULT_CONFIG_FILE_PATH);
		webServer.loop(io);
	}
	else if (ac == 2)
	{
		WebServer webServer(av[1]);
		webServer.loop(io);
	}
	else
	{
		std::cerr << "Error: please enter only the .conf file as the first argument (./webserv [.conf])" << std::endl;
	}
}
