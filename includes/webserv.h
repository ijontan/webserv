
#ifndef WEBSERV_H
#define WEBSERV_H

// macros
#define DEFAULT_CONFIG_FILE_PATH "config_files/default.conf"

#include <Server.hpp>

// libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include "colors.h"
#include <arpa/inet.h>
#include <fcntl.h>


// data structures
#include <vector>
#include <map>
#include <map>

// header files
#include "colors.h"

// hpp files
#include "ABlock.hpp"
#include "Parser.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"
#include "WebServer.hpp"
#include "CustomException.hpp"
#include "IOAdaptor.hpp"
#include "NetIO.hpp"

#endif
