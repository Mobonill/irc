/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:54:19 by lchauffo          #+#    #+#             */
/*   Updated: 2025/07/24 14:26:05 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void addClientsFd(const Channel &channel, std::set<int> &all_clients_fd)
{
	for (std::set<Client *>::const_iterator sit = channel.getListClients().begin();
	sit != channel.getListClients().end(); ++sit)
		all_clients_fd.insert((*sit)->getSocket());
}

const std::string Server::privmsgMsg(int client_fd, const std::vector<std::string> &priv, const std::string &target)
{
	std::cout << "inside privMsg\n";
	std::string null_str = "";

	std::map<int, Client>::const_iterator found_client = _clients.find(client_fd);
	if (found_client == _clients.end())
		return null_str;
	Client this_client = found_client->second;
	std::string client_full_address = this_client.getNickName() + "!" + this_client.getUserName() + "@" + this_client.getIp();
	std::cout << client_full_address << std::endl;
	std::string fullmsg = priv.back();
	std::string msg = std::string(":") + client_full_address + " PRIVMSG " + target + " " + fullmsg + "\r\n";
	return msg;
}

// const std::string botMsg(const std::string &clientNick, const std::string &msg)
// {
// 	std::string botAddress = " \x031 D-CODER!BOT\x0F@localhost";//" D-CODER!BOT@localhost" in color
// 	std::string cmd = "\x02 PRIVMSG\x0F ";//" PRIVMSG " in BOLD
// 	std::string fullMsg = std::string(":") + botAddress + cmd + clientNick + " :" + msg + botEnd;
// 	return fullMsg;
// }

void Server::sendMsgListClients(const std::vector<std::string> &cmd, const std::set<int> &list_clients, \
int client_fd, std::string msg)
{
	for (std::set<int>::iterator fd = list_clients.begin(); fd != list_clients.end(); ++fd)
	{
		if (msg.empty())
			msg = privmsgMsg(client_fd, cmd, _clients[*fd].getNickName());
		if (!msg.empty() && *fd != client_fd)
			send(*fd, msg.c_str(), msg.size(), 0);
	}
}

// #*com*p*l*ex*
bool	wildcardMatch(const std::string &str, const std::string &pattern)
{
	size_t p = 0;
	size_t s = 0;
	size_t match = 0;
	size_t last_wild_card = std::string::npos;
	size_t p_size = pattern.size();
	size_t s_size = str.size();

	while (s < s_size)
	{
		if (p < p_size && pattern[p] == str[s]) {p++; s++;}
		else if (p < p_size && pattern[p] == '*') {last_wild_card = p++; match = s;}
		else if (last_wild_card != std::string::npos) {p = last_wild_card + 1; s = ++match;}
		else return false;
	}
	while (p < p_size && pattern[p] == '*') p++;
	return p==pattern.size();
}

// std::string	rebuildMessage(const std::vector<std::string> &split_msg)
// {
// 	std::string	msg = "";
// 	size_t word_size = 0;
// 	size_t break_line_pos = std::string::npos;

// 	std::vector<std::string>::const_iterator word;
// 	for (word = split_msg.begin(); word != split_msg.end(); ++word)
// 		if ((*word)[0] == ':')
// 			break;
// 	for (;word != split_msg.end(); ++word)
// 	{
// 		break_line_pos = word->find("\r\n");
// 		if (break_line_pos == std::string::npos)
// 			word_size = word->size();
// 		else
// 			word_size = break_line_pos;
// 		for (size_t st = 0; st < word_size; ++st)
// 		{
// 			char c = (*word)[st];
// 			if (std::isprint(static_cast<unsigned char>(c)) != 0)
// 				msg += c;
// 			else if (c == '\t')
// 				msg += ' ';
// 		}
// 		if (break_line_pos != std::string::npos)
// 			break ;
// 		if ((word + 1) != split_msg.end())
// 			msg += " ";
// 	}
// 	return msg;
// }
