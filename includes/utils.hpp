#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace utils
{
std::vector<std::string> split(std::string s, char c);
std::vector<std::string> split(std::string s, std::string delS);
std::pair<std::string, std::string> splitPair(std::string s, std::string delS);
template <typename T>
bool find(std::vector<T> arr, T value)
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

int stoi(std::string s, int lineNum);
std::string to_string(int value);
std::string join(std::vector<std::string> strs, std::string sep, size_t n);
} // namespace utils
