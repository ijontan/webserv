
#include "IOAdaptor.hpp"
#include "NetIO.hpp"
#include "MethodIO.hpp"
#include "webserv.h"
#include <vector>

int main(int ac, char **av)
{
	// (void)ac;
	// (void)av;
	// struct addrinfo hints, *servInfo, *p;
	// std::vector<std::string> ports;
	//
	// ports.push_back("8080");
	// ports.push_back("8081");
	// ports.push_back("8082");
	//
	// memset(&hints, 0, sizeof(hints));
	// hints.ai_family = AF_UNSPEC;	 // AF_INET or AF_INET6 to force version
	// hints.ai_socktype = SOCK_STREAM; // TCP
	// hints.ai_flags = AI_PASSIVE;
	//
	// for (std::vector<std::string>::iterator it = ports.begin(); it !=
	// ports.end(); it++) { 	if (getaddrinfo(NULL, (*it).c_str(), &hints,
	// &servInfo) != 0)
	// 	{
	// 		std::cerr << "getaddrinfo error" << std::endl;
	// 		throw "getaddrinfo error";
	// 	}
	// }

	MethodIO io;
	if (ac == 1)
	{
		WebServer webServer(DEFAULT_CONFIG_FILE_PATH, io);
		webServer.loop();
	}
	else if (ac == 2)
	{
		WebServer webServer(av[1], io);
		webServer.loop();
	}
	else
	{
		std::cerr << "Error: please enter only the .conf file as the first "
					 "argument (./webserv [.conf])"
				  << std::endl;
	}
}
