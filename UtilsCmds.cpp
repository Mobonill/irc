/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:54:19 by lchauffo          #+#    #+#             */
/*   Updated: 2025/07/17 13:20:07 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const std::string rebuildMessage(const std::vector<std::string> &splitMsg);

void addClientsFd(const Channel &channel, std::set<int> &allClientsFd)
{
	for (std::set<Client *>::const_iterator sit = channel.getListClients().begin();
	sit != channel.getListClients().end(); ++sit)
		allClientsFd.insert((*sit)->getClientSocket());
}

const std::string Server::privmsgMsg(int clientFd, const std::vector<std::string> &priv, const std::string &target)
{
	std::map<int, Client>::const_iterator foundClient = _clients.find(clientFd);
	std::cout << "inside privMsg\n";
	std::string nullStr = "";
	if (foundClient == _clients.end())
		return nullStr;
	Client thisClient = foundClient->second;
	std::string clientFullAddress = thisClient.getNickName() + "!" + thisClient.getUserName() + "@" + thisClient.getIp();
	std::cout << clientFullAddress << std::endl;
	std::string fullmsg = rebuildMessage(priv);
	std::string msg = std::string(":") + clientFullAddress + " PRIVMSG " + target + " " + fullmsg + "\r\n";
	return msg;
}

const std::string botMsg(const std::string &clientNick, const std::string &msg)
{
	std::string botAddress = " \x031 D-CODER!BOT\x0F@localhost";//" D-CODER!BOT@localhost" in color
	std::string cmd = "\x02 PRIVMSG\x0F ";//" PRIVMSG " in BOLD
	std::string fullMsg = std::string(":") + botAddress + cmd + clientNick + " :" + msg + botEnd;
	return fullMsg;
}

void Server::sendMsgListClients(const std::vector<std::string> &cmd, const std::set<int> &listClients, int clientFd, std::string msg)
{
	for (std::set<int>::iterator fd = listClients.begin(); fd != listClients.end(); ++fd)
	{
		if (msg.empty())
			msg = privmsgMsg(clientFd, cmd, _clients[*fd].getNickName());
		if (!msg.empty() && *fd != clientFd)
			send(*fd, msg.c_str(), msg.size(), 0);
	}
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
