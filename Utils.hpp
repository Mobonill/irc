// Utils.hpp
#pragma once
#include <string>
#include <vector>

std::vector<std::string> splitString(const std::string &str, const char *delim);
bool isValid(char c, const std::string &validChars);
bool onlyValid(const std::string &str, const std::string &validChars);
