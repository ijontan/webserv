#include "utils.hpp"
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> utils::split(std::string s, char c)
{
	std::istringstream iss(s);
	std::string token;
	std::vector<std::string> ret;

	while (std::getline(iss, token, c))
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

template <typename T>
bool utils::find(std::vector<T> arr, T value)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] == value)
		{
			return true;
		}
	}
	return false;
}
