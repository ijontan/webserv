#pragma once

#include <string>
#include <dirent.h>

class AutoIndex {
public:
	AutoIndex(void);
	AutoIndex(std::string path, std::string queryPath);
	~AutoIndex(void);
	AutoIndex(const AutoIndex &src);
	AutoIndex	&operator=(const AutoIndex &rhs);
	std::string getBody();

private:
	std::string getIndexes();
	DIR *_dir;
	std::string _queryPath;
	std::string _path;
};
