#pragma once
#include <string>
#include <vector>

// Utils.cpp
const std::string isValidPassword(const std::string &password);
std::vector<std::string> splitString(const std::string& str, const char* delimiter);
bool isValid(const char &c, const std::string &valid);
bool onlyValid(const std::string &s, const std::string &valid);

// UtilsCmds.cpp
void addClientsFd(const Channel &channel, std::set<int> &all_clients_fd);
bool wildcardMatch(const std::string &str, const std::string &pattern);
// std::string rebuildMessage(const std::vector<std::string> &msg);

// Commands.cpp
bool summonBot(const std::string &msg);
