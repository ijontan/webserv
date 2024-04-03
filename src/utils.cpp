#include "utils.hpp"
#include "CustomException.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> utils::split(std::string s, char c)
{
	std::istringstream iss(s);
	std::string token;
	std::vector<std::string> ret;

	while (std::getline(iss, token, c))
		if (!token.empty())
			ret.push_back(token);
	return ret;
}

std::vector<std::string> utils::split(std::string s, std::string delS)
{
	std::vector<std::string> ret;
	int end = s.find(delS);

	while (end != -1)
	{
		ret.push_back(s.substr(0, end));
		s.erase(s.begin(), s.begin() + end + delS.size());
		end = s.find(delS);
	}
	ret.push_back(s.substr(0, end));
	return ret;
}

std::pair<std::string, std::string> utils::splitPair(std::string s, std::string delS)
{
	size_t end = s.find(delS);
	if (end == std::string::npos)
		return std::pair<std::string, std::string>(s, "");
	std::pair<std::string, std::string> pair(s.substr(0, end), s.substr(end + delS.size()));
	return pair;
}

int utils::stoi(std::string s, int lineNum)
{
	std::stringstream ss(s);
	int value;

	ss >> value;
	if (ss.fail() && lineNum != -1)
	{
		std::stringstream ss;
		ss << "Error (line " << lineNum
		<< "): int out of range (0 to 2147483647)";
		throw CustomException(ss.str());
	}
	return value;
}

std::string utils::to_string(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string utils::join(std::vector<std::string> strs, std::string sep, size_t n)
{
	std::ostringstream ss;

	for (size_t i = 0; i < n && i < strs.size(); i++)
		ss << sep << strs[i];
	return ss.str();
}
