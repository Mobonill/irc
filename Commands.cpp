/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:36:07 by lchauffo          #+#    #+#             */
/*   Updated: 2025/07/29 11:31:18 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::sendServerMessage(int client_fd, const std::string &code, const std::string &entry)
{
	std::string client_name = _clients[client_fd].getNickName();
	std::string full_msg = ":" + _server_name + " " + code + " " + client_name + " " + entry + "\r\n";
	send(client_fd, full_msg.c_str(), full_msg.size(), 0);
}

void	Server::checkVersion()
{
	std::cout << "I don't ****ing know\n";
}

void	Server::checkStatus(int client_fd)
{
	std::string status = "NOT_AUTHENTCATD";
	int st = _clients[client_fd].getStatus();
	if (st == NOT_REGISTRD)
		status = "NOT_REGISTRD";
	else if (st == REGISTRD)
		status = "REGISTRD";
	else if (st == IN_CHANNEL)
		status = "IN_CHANNEL";
	std::cout << "Client " << client_fd << " - status: " << status << std::endl;
	return sendServerMessage(client_fd, "", std::string("STATUS :") + status);
}

void	Server::checkPass(const std::vector<std::string> &pass, int client_fd)
{
	if (pass.size() < 2)
		return sendServerMessage(client_fd, "461", "PASS :Not enough parameters");
	else if (pass.size() > 2)
		return sendServerMessage(client_fd, "461", "PASS :Too many parameters");
	else if (_clients[client_fd].getAuthenticated() == true)
		return sendServerMessage(client_fd, "462", "PASS :You may not reregister");
	else if (pass[1].size() > 128) //OWASP Authentication Cheat Sheet, can be down to 64 chars
		return sendServerMessage(client_fd, "464", "PASS :Password incorrect");
	else if ((pass[1][0] == ':' &&  _password.compare(&pass[1][1]) != 0))
		return sendServerMessage(client_fd, "464", "PASS :Password incorrect");
	else if (pass[1][0] != ':' && pass[1].compare(_password) != 0)
		return sendServerMessage(client_fd, "464", "PASS :Password incorrect");
	else
	{
		_clients[client_fd].setAuthenticated(true);
		_clients[client_fd].setStatus(NOT_REGISTRD);
		std::cout << "Client " << client_fd << " authenticated successfully." << std::endl;
	}
	return ;
}

void	Server::checkNick(const std::vector<std::string> &nick, int client_fd)
{
	if (nick.empty() || nick.size() < 2)
		return sendServerMessage(client_fd, "431", "NICK :No nickname given");
	else if (nick.size() > 2)
		return sendServerMessage(client_fd, "432", "NICK :Erroneus nickname");
	else
	{
		if (nick[1].size() > 9 || (!std::isalpha(nick[1][0]) && !isValid(nick[1][0], SPECIAL))
		|| !onlyValid(&nick[1][1], ALPHANUMSPE))
			return sendServerMessage(client_fd, "432", "NICK :Erroneus nickname");
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			if (it->second.getNickName().compare(nick[1]) == 0)
				return sendServerMessage(client_fd, "433", "NICK :Nickname is already in use");
		std::string old_nick = _clients[client_fd].getNickName();
		_clients[client_fd].setNickName(nick[1]);
		if (old_nick == "*")
		{
			std::cout << "Introducing new nick \"" << _clients[client_fd].getNickName() << "\"." << std::endl;
			if (!_clients[client_fd].getUserName().empty())
				_clients[client_fd].setStatus(REGISTRD);
		}
		else
		{
			std::string msg = ":" + old_nick + " NICK " + _clients[client_fd].getNickName() + "\r\n";
			std::set<int> same_channels_clients;
			for (std::map<std::string, Channel> ::const_iterator mit = _clients[client_fd].getJoinedChannels().begin();
			mit != _clients[client_fd].getJoinedChannels().end(); ++mit)
				addClientsFd(mit->second, same_channels_clients);
			sendMsgListClients(nick, same_channels_clients, client_fd, msg);
		}
	}
}

// input a name to make a guest an actual client
// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
// Parameters: <username> <hostname> <servername> <realname>
void	Server::checkUser(const std::vector<std::string> &user, int client_fd)//Zara this is a fake version just for testing
{
	for (std::vector<std::string>::const_iterator vit = user.begin(); vit != user.end(); ++vit)
		std::cout << *vit << std::endl;
	_clients[client_fd].setServerName("patate");
	_clients[client_fd].setUserName("lchauffo");
	_clients[client_fd].setHostName("lulu");
	_clients[client_fd].setRealName("lucie");
	if (_clients[client_fd].getNickName() != "*")
		_clients[client_fd].setStatus(REGISTRD);
}

// JOIN #[name of channel here]
// can't start any discussion if no channel created, even a general one
// no user == no channel, only at the start of a server
// void	checkJoin(int fd, std::string cmd)//Zara
// {}

// void	checkTopic(int fd, std::string cmd)//Zara
// {}

// void	checkKick(int fd, std::string cmd)//Zara
// {}

// void	checkMode(int fd, std::string cmd)//Zara
// {}

void	Server::checkInfo(const std::vector<std::string> &info, int client_fd)//Lulu
{
	if (info.size() >= 2 && info[1] != _server_name)
		return sendServerMessage(client_fd, "402", info[1] + " :No such server");//to be verified
	sendServerMessage(client_fd, "373", ":INFO start");
	sendServerMessage(client_fd, "371", ":Server name: " + _server_name);
	sendServerMessage(client_fd, "371", ":Port: " + toString(_port));
	std::string err = ":Compiled on: " + std::string(__DATE__) + " " + __TIME__;
	sendServerMessage(client_fd, "371", err);
	sendServerMessage(client_fd, "374", ":End of /INFO list");
}

// void	checkInvite(int fd, std::string cmd)//Zara
// {}

void	Server::privToChannel(const std::vector<std::string> &priv, int client_fd)
{
	if (priv[2][0] != ':')
		return sendServerMessage(client_fd, "412", "PRIVMSG :No text to send");
	std::map<std::string, Channel>::iterator this_channel = _channels.find(priv[1]);
	std::string msg = privmsgMsg(client_fd, priv, priv[1]);
	if (this_channel != _channels.end())
	{
		std::set<int> this_channel_clients;
		addClientsFd(this_channel->second, this_channel_clients);
		sendMsgListClients(priv, this_channel_clients,client_fd, msg);
		return ;
	}
	else
	{
		size_t find_match_channel = 0;
		size_t operator_status = 0;
		if (priv[1].find('*') != std::string::npos)
		{
			if (priv[1][2] == '*')
			{
				std::set<int> all_match_channels_clients;
				for (std::map<std::string, Channel>::iterator mit = _channels.begin();
					mit != _channels.end(); ++mit)
				{
					if (!mit->second.getClientPriviledge(client_fd).empty())
						if (wildcardMatch(mit->first, priv[1]) == true && ++find_match_channel)
							if (mit->second.getClientPriviledge(client_fd).find('@') != std::string::npos && ++operator_status)
								addClientsFd(mit->second, all_match_channels_clients);
				}
				sendMsgListClients(priv, all_match_channels_clients,client_fd, msg);
			}
		}
		if (find_match_channel == 0)
			return sendServerMessage(client_fd, "401", "PRIVMSG "+ priv[1] + " :No such nick/channel");
		if (operator_status == 0)
			return sendServerMessage(client_fd, "481", "PRIVMSG "+ priv[1] + " :Permission Denied- You're not an IRC operator");
	}
}

void	Server::privToClient(const std::vector<std::string> &priv, int client_fd)
{
	std::vector<std::string> multi_clients = splitString(priv[1], ",");
	if (multi_clients.size() > 15)
		return sendServerMessage(client_fd, "407", "PRIVMSG "+ priv[1] + " :Duplicate recipients. No message delivered");
	std::set<int> existing_clients;
	std::set<std::string> not_found;
	for (std::vector<std::string>::iterator this_client = multi_clients.begin(); this_client != multi_clients.end(); ++this_client)
	{
		std::map <int, Client>::iterator mit;
		for (mit = _clients.begin(); mit != _clients.end(); ++mit)
			if (*this_client == mit->second.getNickName())
				break ;
		if (mit == _clients.end())
			not_found.insert(*this_client);
		else
			existing_clients.insert(mit->first);
	}
	std::cout << "Size: " << existing_clients.size() << " existing clients" << std::endl;
	sendMsgListClients(priv, existing_clients, client_fd, "");
	std::cout << "Size: " << not_found.size() << " notfound clients" << std::endl;
	for (std::set<std::string>::iterator sit = not_found.begin(); sit != not_found.end(); ++sit)
		sendServerMessage(client_fd, "401", "PRIVMSG "+ *sit + ":No such nick/channel");
}

// :nickname!username@hostname
void	Server::checkPrivmsg(const std::vector<std::string> &priv, int client_fd)//Lulu
{
	if (priv.size() == 1)
		return sendServerMessage(client_fd, "411", "PRIVMSG :No recipient given");
	else if (priv[1][0] == ':')
		return sendServerMessage(client_fd, "411", "PRIVMSG :No recipient given");
	else if (priv.size() < 3 || priv[2][0] != ':')
		return sendServerMessage(client_fd, "412", "PRIVMSG :No text to send");
	if (priv[1][0] == '#')
		privToChannel(priv, client_fd);
	else
		privToClient(priv, client_fd);
}

bool summonBot(const std::string &msg)//check the presence of one summon_expression in a message to initiate a bot conversation
{
	if (msg.empty())
		return false;
	std::string summon_expressions = "divination,taro,futur,42forecast,DE-CODER,Dcoder,decoder,read me,akinator";
	std::vector<std::string> summon_bot = splitString(summon_expressions, ",");
	std::vector<std::string>::iterator bot_summoned;
	for (bot_summoned = summon_bot.begin(); bot_summoned != summon_bot.end(); ++bot_summoned)
		if (msg.find(*bot_summoned) != std::string::npos)
			return true;
	return false;
}
#if BONUS
void	Server::checkBot(const std::vector<std::string> &bot, int client_fd)//Lulu
{
	std::cout << "DEBUG: checkBot called with " << bot.size() << " parameters" << std::endl;

	bool in_channel = false;
	std::string channel_name = "";
	std::string client_name = "";
	std::string msg = "";
	std::cout << "- before assignement\n";
	if (bot.size() > 2)
	{
		std::cout << "-- if bot.size() > 2\n";
		channel_name = bot[1];
		msg = bot[2];
	}
	else if (bot.size() == 2)
	{
		std::cout << "-- if bot.size() == 2\n";
		if (bot[1][0] == ':')
			msg = bot[1];
		else
			channel_name = bot[1];
	}
	std::cout << "- after ifs\n";
	std::cout << "- channel_name = [" << channel_name << "]\n";
	std::cout << "- msg = [" << msg << "]\n";
	std::map<int, Client>::iterator this_client = _clients.find(client_fd);
	if (this_client != _clients.end())
		client_name = this_client->second.getNickName();
	std::cout << "- client_name = [" << client_name << "]\n";
	_bot->handleConversation(client_fd, msg, client_name, channel_name, in_channel);

	std::cout << "end of function checkBot()\n";
}
#endif

// 16/9 / 1080p / 60img/sec / use printsc / loop / son ok