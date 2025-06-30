/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:54:19 by lchauffo          #+#    #+#             */
/*   Updated: 2025/06/30 17:31:09 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void addClientsFd(const Channel &channel, std::set<int> allClientsFd);
const std::string &privmsgMsg(const Client &client, std::vector<std::string> priv);
void sendMsgListClients(std::set<int> listClients, const int &clientFd, const std::string &msg);
bool	wildcardMatch(const std::string &str, const std::string &pattern);
std::string	rebuildMessage(const std::vector<std::string> &msg);


void addClientsFd(const Channel &channel, std::set<int> allClientsFd)
{
	for (const std::set<Client>::iterator sit = channel.getListClients().begin();
	sit != channel.getListClients().end(); ++sit)
		allClientsFd.insert(sit->getClientSocket());
}

const std::string &privmsgMsg(const Client &client, std::vector<std::string> priv)
{
	std::string clientFullAddress = client.getNickName() + "!" + client.getUserName() + "@" + client.getIp();
	std::string msg = ":" + clientFullAddress + " PRIVMSG " + priv[1][0] + " " + rebuildMessage(priv);
	return msg;
}

const std::string &colorWrap(const std::string &colorcode, const std::string &msg)
{
	// returns a string like \x03XXmsg\x0F
	// send(fd, "\x0304The Bug whispers... segfault awaits.\x0F\r\n", ...);
	// ex: "\x02\x0310[TarotBot]\x0F \x0312You drew \x02The Forked Path\x0F"

}

const std::string &botMsg(const std::string &clientNick, const std::string &msg)
{
	std::string fullMsg = ":" + " \x03D-CODER!BOT\x0F@localhost" + " \x02PRIVMSG\x0F " + clientNick + " :" + msg + "\r\n";
	return fullMsg;
}

void sendMsgListClients(std::set<int> listClients, const int &clientFd, const std::string &msg)
{
	for (std::set<int>::iterator fd = listClients.begin(); fd != listClients.end(); ++fd)
	if (*fd != clientFd)
		send(*fd, msg.c_str(), msg.size(), 0);
}

// #*com*p*l*ex*
bool	wildcardMatch(const std::string &str, const std::string &pattern)
{
	size_t p = 0;
	size_t s = 0;
	size_t match = 0;
	size_t lastWildCard = std::string::npos;
	size_t pSize = pattern.size();
	size_t sSize = str.size();

	while (s < sSize)
	{
		if (p < pSize && pattern[p] == str[s]) {p++; s++;}
		else if (p < pSize && pattern[p] == '*') {lastWildCard = p++; match = s;}
		else if (lastWildCard != std::string::npos) {p = lastWildCard + 1; s = ++match;}
		else return false;
	}
	while (p < pSize && pattern[p] == '*') p++;
	return p==pattern.size();
}

const std::string	rebuildMessage(const std::vector<std::string> &splitMsg)
{
	std::string	msg;
	size_t wordSize = 0;
    size_t breaklinePos = std::string::npos;

	std::vector<std::string>::const_iterator word;
	for (word = splitMsg.begin(); word != splitMsg.end(); ++word)
		if ((*word)[0] == ':')
			break;
	for (;word != splitMsg.end(); ++word)
	{
		breaklinePos = word->find("\r\n");
		if (breaklinePos == std::string::npos)
			wordSize = word->size();
		else
			wordSize = breaklinePos;
		for (size_t st = 0; st < wordSize; ++st)
		{
			char c = (*word)[st];
			if (std::isprint(static_cast<unsigned char>(c)) != 0)
				msg += c;
			else if (c == '\t')
				msg += ' ';
		}
		if (breaklinePos != std::string::npos)
			break ;
		if ((word + 1) != splitMsg.end())
			msg += " ";
	}
	return msg;
}
