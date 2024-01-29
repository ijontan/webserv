
#include "Webserv.hpp"

int main(int ac, char const *av[])
{
	Parser parser;

	if (ac == 1) {
		parser.parseConfigFile("default.conf");
	} else (ac == 2) {
		parser.parseConfigFile(av[1]);
	} else {
		std::cerr << "Error: please only enter the .conf file (./webserv [.conf])" << std::endl;
	}
}
