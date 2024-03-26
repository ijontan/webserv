#include "AutoIndex.hpp"
#include "RequestException.hpp"
#include <dirent.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

/***********************************
 * Constructors
 ***********************************/

AutoIndex::AutoIndex(void)
{
}
AutoIndex::AutoIndex(std::string path, std::string queryPath)
{
	_dir = opendir(path.c_str());
	std::cout << path << std::endl;
	if (!_dir)
		throw RequestException("Failed to open Dir", 301);
	_path = path;
	_queryPath = queryPath;
}

AutoIndex::AutoIndex(const AutoIndex &src)
{
	(void)src;
}

AutoIndex &AutoIndex::operator=(const AutoIndex &rhs)
{
	(void)rhs;
	return *this;
}

/***********************************
 * Destructors
 ***********************************/

AutoIndex::~AutoIndex(void)
{
	closedir(_dir);
}

/***********************************
 * Others
 ***********************************/
std::string AutoIndex::getIndexes()
{
	struct dirent *entry;
	std::ostringstream ss;

	while ((entry = readdir(_dir)) != NULL)
	{
		std::string name(entry->d_name);
		name = name + (entry->d_type == DT_DIR ? "/" : "");
		ss << "<p><a href=\"" << _queryPath << name << "\">" << name << "</a></p>\n";
	}
	return ss.str();
}

std::string AutoIndex::getBody()
{
	std::ostringstream ss;

	ss << "<!DOCTYPE html>\n"
	   << "<html lang=\"en\">\n"
	   << "<head>\n"
	   << "<title>" << _queryPath << "</title>\n"
	   << "<meta charset=\"UTF-8\">\n"
	   << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
	   << "</head>\n"
	   << "<body>\n";
	ss << "<h1>" << _path << "</h1>\n";
	// ss << "<p>\n";
	ss << getIndexes();
	// ss << "</p>\n";
	ss << "</body>\n"
	   << "</html>\n";

	return ss.str();
}
