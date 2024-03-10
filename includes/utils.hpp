#pragma once

#include <string>
#include <utility>
#include <vector>

namespace utils
{
std::vector<std::string> split(std::string s, char c);
std::vector<std::string> split(std::string s, std::string delS);
std::pair<std::string, std::string> splitPair(std::string s, std::string delS);
template <typename T>
bool find(std::vector<T> arr, T value);
int stoi (std::string s);
std::string to_string(int value);
} // namespace utils

