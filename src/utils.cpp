#include "utils.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

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
	int end = s.find(delS);
	if (end == -1)
		return std::pair<std::string, std::string>(s, "");
	std::pair<std::string, std::string> pair(s.substr(0, end), s.substr(end + 2));
	return pair;
}

int utils::stoi(std::string s)
{
	std::stringstream ss(s);
	int value;

	ss >> value;
	if (ss.fail())
		throw "Not and integer";
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
