#pragma once

#define X03D std::string("\x03D")
#define X01DX034 std::string("\x01D\x034")
#define X01DX02X03 std::string("\x01D \x02 \x03")

#define ITALIC std::string("\x01D")
#define BOLD std::string("\x02")
#define COLOR std::string("\x03")
#define RST "\x0F"
#define SPOILER COLOR + "01,01"
#define LBLUE COLOR + "12"
#define PINK COLOR + "13"
#define BLU COLOR + "58"
#define BGREEN COLOR + "74"
#define DBLUE COLOR + "33"
#define GREY COLOR + "30"
#define RDM COLOR + "30"

// Utils.cpp
std::vector<std::string> splitString(const std::string& str, const char* delimiter);
bool isValid(const char &c, const std::string &valid);
bool onlyValid(const std::string &s, const std::string &valid);

// UtilsCmds.cpp
void addClientsFd(const Channel &channel, std::set<int> &allClientsFd);
bool wildcardMatch(const std::string &str, const std::string &pattern);
std::string rebuildMessage(const std::vector<std::string> &msg);
const std::string botMsg(const std::string &clientNick, const std::string &msg);

// Commands.cpp
bool summonBot(const std::string &msg);
