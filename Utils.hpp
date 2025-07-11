#pragma once

#define X03D std::string("\x03D")
#define X01DX034 std::string("\x01D\x034")

// Utils.cpp
std::vector<std::string> splitString(const std::string& str, const char* delimiter);
bool isValid(const char &c, const std::string &valid);
bool onlyValid(const std::string &s, const std::string &valid);

// UtilsCmds.cpp
void addClientsFd(const Channel &channel, std::set<int> &allClientsFd);
// const std::string &privmsgMsg(const Client *client, const std::vector<std::string> &priv);
// void sendMsgListClients(const std::set<int> &listClients, const int &clientFd, const std::string &msg);
bool	wildcardMatch(const std::string &str, const std::string &pattern);
std::string	rebuildMessage(const std::vector<std::string> &msg);
const std::string &botMsg(const std::string &clientNick, const std::string &msg);

// Commands.cpp
bool summonBot(const std::string &msg);
