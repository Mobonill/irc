#pragma once

#define X03D std::string("\x03D")
#define X01DX034 std::string("\x01D\x034")

// Utils.cpp
std::vector<std::string> splitString(const std::string& str, const char* delimiter);
bool isValid(const char &c, const std::string &valid);
bool onlyValid(const std::string &s, const std::string &valid);

// UtilsCmds.cpp
void addClientsFd(const Channel &channel, std::set<int> allClientsFd);
const std::string &privmsgMsg(const Client &client, std::vector<std::string> priv);
void sendMsgListClients(std::set<int> listClients, const int &clientFd, const std::string &msg);
bool	wildcardMatch(const std::string &str, const std::string &pattern);
std::string	rebuildMessage(const std::vector<std::string> &msg);
const std::string &botMsg(const std::string &clientNick, const std::string &msg);

// Commands.cpp
void	privToChannel(std::vector<std::string> priv, int clientFd, std::map<std::string, Channel> serverChannels, std::map<int, Client> serverClients);
void	privToClient(std::vector<std::string> priv, int clientFd, std::map<int, Client> serverClients);

bool summonBot(const std::string &msg);
