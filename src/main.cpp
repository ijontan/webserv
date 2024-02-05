
#include "webserv.h"

int main(int ac, char **av)
{
	if (ac == 1) {
		WebServer webServer(DEFAULT_CONFIG_FILE_PATH);
	} else if (ac == 2) {
		WebServer webServer(av[1]);
	} else {
		std::cerr << "Error: please enter only the .conf file as the first argument (./webserv [.conf])" << std::endl;
	}
}
