/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:36:07 by lchauffo          #+#    #+#             */
/*   Updated: 2025/07/17 14:21:54 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"
#include <cctype>
#include <fstream>
#include <set>

void	Server::sendServerMessage(int clientFd, const std::string &code, const std::string &entry)
{
	std::string clientName = _clients[clientFd].getNickName();
	std::string fullMessage = ":" + _serverName + " " + code + " " + clientName + " " + entry + "\r\n";
	send(clientFd, fullMessage.c_str(), fullMessage.size(), 0);
}

void	Server::checkVersion(int client_fd)
{
	std::cout << "I don´t fuck*ng know\n";
}

void	Server::checkStatus(int clientFd)
{
	std::string status = "NOT_AUTHENTCATD";
	int st = _clients[clientFd].getStatus();
	if (st == NOT_REGISTRD)
		status = "NOT_REGISTRD";
	else if (st == REGISTRD)
		status = "REGISTRD";
	else if (st == IN_CHANNEL)
		status = "IN_CHANNEL";
	std::cout << "Client " << clientFd << " - status: " << status << std::endl;
	return sendServerMessage(clientFd, "", std::string("STATUS :") + status);
}

void	Server::checkPass(const std::vector<std::string> &pass, int clientFd)
{
	std::cout << "DEBUG: checkPass called with " << pass.size() << " parameters" << std::endl;
	if (pass.size() >= 2)
		std::cout << "DEBUG: Password received: '" << pass[1] << "', Expected: '" << _password << "'" << std::endl;
	if (pass.size() < 2)
		return sendServerMessage(clientFd, "461", "PASS :Not enough parameters");
	else if (pass.size() > 2)
		return sendServerMessage(clientFd, "461", "PASS :Too many parameters");
	else if (_clients[clientFd].getAuthenticated() == true)
		return sendServerMessage(clientFd, "462", "PASS :You may not reregister");
	else if (pass[1].size() > 128) //OWASP Authentication Cheat Sheet, can be down to 64 chars
		return sendServerMessage(clientFd, "464", "PASS :Password incorrect");
	else if (pass[1].compare(_password) != 0)
		return sendServerMessage(clientFd, "464", "PASS :Password incorrect");
	else
	{
		_clients[clientFd].setAuthenticated(true);
		_clients[clientFd].setStatus(NOT_REGISTRD);
		std::cout << "Client " << clientFd << " authenticated successfully." << std::endl;
	}
	return ;
}

void	Server::checkNick(const std::vector<std::string> &nick, int clientFd)
{
	if (nick.empty() || nick.size() != 2)
		return sendServerMessage(clientFd, "431", "NICK :No nickname given");
	else
	{
		if (nick[1].size() > 9 || (!std::isalpha(nick[1][0]) && !isValid(nick[1][0], SPECIAL))
		|| !onlyValid(&nick[1][1], ALPHANUMSPE))
			return sendServerMessage(clientFd, "432", "NICK :Erroneus nickname");
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			if (it->second.getNickName().compare(nick[1]) == 0)
				return sendServerMessage(clientFd, "433", "NICK :Nickname is already in use");
		std::string oldNick = _clients[clientFd].getNickName();
		_clients[clientFd].setNickName(nick[1]);
		if (oldNick == "*")
		{
			std::cout << "Introducing new nick \"" << _clients[clientFd].getNickName() << "\"." << std::endl;
			if (!_clients[clientFd].getUserName().empty())
				_clients[clientFd].setStatus(REGISTRD);
		}
		else
		{
			std::string msg = ":" + oldNick + " NICK " + _clients[clientFd].getNickName() + "\r\n";
			std::set<int> sameChannelsClients;
			for (std::map<std::string, Channel> ::const_iterator mit = _clients[clientFd].getJoinedChannels().begin();
			mit != _clients[clientFd].getJoinedChannels().end(); ++mit)
				addClientsFd(mit->second, sameChannelsClients);
			sendMsgListClients(nick, sameChannelsClients, clientFd, msg);
		}
	}
}

// input a name to make a guest an actual client
// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
// Parameters: <username> <hostname> <servername> <realname>
void	Server::checkUser(const std::vector<std::string> &user, int clientFd)//Morgane
{
	std::cout << "**********DEBUG: checkUser called with " << user.size() << " parameters" << std::endl;
	for (std::vector<std::string>::const_iterator vit = user.begin(); vit != user.end(); ++vit)
		std::cout << *vit << std::endl;
	_clients[clientFd].setServerName("patate");
	_clients[clientFd].setUserName("lchauffo");
	_clients[clientFd].setHostName("lulu");
	_clients[clientFd].setRealName("lucie");
	(void)user;
	(void)clientFd;
	if (_clients[clientFd].getNickName() != "*")
		_clients[clientFd].setStatus(REGISTRD);
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

void	Server::checkInfo(const std::vector<std::string> &info, int clientFd)//Lulu
{
	if (info.size() >= 2 && info[1] != _serverName)
		return sendServerMessage(clientFd, "402", info[1] + " :No such server");//to be verified
	sendServerMessage(clientFd, "373", ":INFO start");
	sendServerMessage(clientFd, "371", ":Server name: " + _serverName);
	sendServerMessage(clientFd, "371", ":Port: " + toString(_port));
	std::string err = ":Compiled on: " + std::string(__DATE__) + " " + __TIME__;
	sendServerMessage(clientFd, "371", err);
	sendServerMessage(clientFd, "374", ":End of /INFO list");
}

// void	checkInvite(int fd, std::string cmd)//Zara
// {}

void	Server::privToChannel(const std::vector<std::string> &priv, int clientFd)
{
	std::cout << "PrivToChannel\n";
	if (priv[2][0] != ':')
		return sendServerMessage(clientFd, "412", "PRIVMSG :No text to send");
	std::map<std::string, Channel>::iterator thisChannel = _channels.find(priv[1]);
	std::string msg = privmsgMsg(clientFd, priv, priv[1]);
	if (thisChannel != _channels.end())
	{
		std::set<int> thisChannelClients;
		addClientsFd(thisChannel->second, thisChannelClients);
		sendMsgListClients(priv, thisChannelClients,clientFd, msg);
		return ;
	}
	else
	{
		size_t findMatchChannel = 0;
		size_t operatorStatus = 0;
		if (priv[1].find('*') != std::string::npos)
		{
			if (priv[1][2] == '*')
			{
				std::set<int> allMatchChannelsClients;
				for (std::map<std::string, Channel>::iterator mit = _channels.begin();
					mit != _channels.end(); ++mit)
				{
					if (!mit->second.getClientPriviledge(clientFd).empty())
						if (wildcardMatch(mit->first, priv[1]) == true && ++findMatchChannel)
							if (mit->second.getClientPriviledge(clientFd).find('@') != std::string::npos && ++operatorStatus)
								addClientsFd(mit->second, allMatchChannelsClients);
				}
				sendMsgListClients(priv, allMatchChannelsClients,clientFd, msg);
			}
		}
		if (findMatchChannel == 0)
			return sendServerMessage(clientFd, "401", "PRIVMSG "+ priv[1] + " :No such nick/channel");
		if (operatorStatus == 0)
			return sendServerMessage(clientFd, "481", "PRIVMSG "+ priv[1] + " :Permission Denied- You're not an IRC operator");
	}
}

void	Server::privToClient(const std::vector<std::string> &priv, int clientFd)
{
	std::cout << "PrivToClient - dest: " << priv[1] << std::endl;
	std::vector<std::string> multiClients = splitString(priv[1], ",");
	if (multiClients.size() > 15)
		return sendServerMessage(clientFd, "407", "PRIVMSG "+ priv[1] + " :Duplicate recipients. No message delivered");
	std::set<int> existingClients;
	std::set<std::string> notFound;
	for (std::vector<std::string>::iterator thisClient = multiClients.begin(); thisClient != multiClients.end(); ++thisClient)
	{
		std::map <int, Client>::iterator mit;
		for (mit = _clients.begin(); mit != _clients.end(); ++mit)
			if (*thisClient == mit->second.getNickName())
				break ;
		if (mit == _clients.end())
			notFound.insert(*thisClient);
		else
			existingClients.insert(mit->first);
	}
	std::cout << "Size: " << existingClients.size() << " existing clients" << std::endl;
	sendMsgListClients(priv, existingClients, clientFd, "");
	std::cout << "Size: " << notFound.size() << " notfound clients" << std::endl;
	for (std::set<std::string>::iterator sit = notFound.begin(); sit != notFound.end(); ++sit)
		sendServerMessage(clientFd, "401", "PRIVMSG "+ *sit + ":No such nick/channel");
}

// :nickname!username@hostname
void	Server::checkPrivmsg(const std::vector<std::string> &priv, int clientFd)//Lulu
{
	std::cout << "DEBUG: checkPrivmsg called with " << priv.size() << " parameters" << std::endl;
	if (priv.size() == 1)
		return sendServerMessage(clientFd, "411", "PRIVMSG :No recipient given");
	else if (priv[1][0] == ':')
		return sendServerMessage(clientFd, "411", "PRIVMSG :No recipient given");
	else if (priv.size() < 3)
		return sendServerMessage(clientFd, "412", "PRIVMSG :No text to send");
	if (priv[1][0] == '#')
		privToChannel(priv, clientFd);
	else
		privToClient(priv, clientFd);
}

bool summonBot(const std::string &msg)//check the presence of one summonExpression in a message to initiate a bot conversation
{
	if (msg.empty())
		return false;
	std::string summonExpressions = "divination,taro,futur,42forecast,DE-CODER,Dcoder,decoder,read me,akinator";
	std::vector<std::string> summonBot = splitString(summonExpressions, ",");
	std::vector<std::string>::iterator vit;
	for (vit = summonBot.begin(); vit != summonBot.end(); ++vit)
		if (msg.find(*vit) != std::string::npos)
			return true;
	return false;
}

void 	Server::bot0(const std::vector<std::string> &bot, const std::string &name, const int &clientFd)//here, client summon BOT directly
{
	std::string composeMsg;
	std::string bmsg;

	std::cout << "-- entering bot [0]" << std::endl;
	std::string rebuilt = rebuildMessage(bot);
	if (bot.size() > 1 && (bot[1][0] != ':' || summonBot(rebuilt) == true))//BOT (ok)|| BOT hello (ko)|| BOT :hello (ko)|| BOT :future (ok)
		return sendServerMessage(clientFd, "421", "BOT :Unknown command");
	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	if (randthree == 0)
		composeMsg = PINK + bot0Call1 + name + bot0Call2 + RST;
	else if (randthree == 1)
		composeMsg = PINK + bot1Call1 + name + bot1Call2 + RST;
	else if (randthree == 2)
		composeMsg = PINK+ bot2Call1 + name + bot2Call2 + RST;
	bmsg = botMsg(name, composeMsg);
	send(clientFd, bmsg.c_str(), bmsg.size(), 0);
	_clients[clientFd].setBotConvStep(2);
}

void 	Server::bot1(const std::string &name, const int &clientFd)//here, client summon BOT indirectly, through keyword detection
{
	std::string composeMsg;
	std::string bmsg;

	std::cout << "-- entering bot [1]" << std::endl;
	composeMsg = BLU + botKeyWordActivate1 + name + botKeyWordActivate2 + RST;
	bmsg = botMsg(name, composeMsg);
	send(clientFd, bmsg.c_str(), bmsg.size(), 0);
	_clients[clientFd].setBotConvStep(2);
}

void 	Server::bot2(const std::string &name, const int &clientFd)//follow with warning and confirmation demand
{
	std::string composeMsg;
	std::string bmsg;

	std::cout << "-- entering bot [2]" << std::endl;
	composeMsg = ITALIC + BOLD + SPOILER + botWarning0 + RST;
	bmsg = botMsg(name, composeMsg);
	send(clientFd, bmsg.c_str(), bmsg.size(), 0);
	composeMsg = RDM + botSummon + RST;
	bmsg = botMsg(name, composeMsg);
	send(clientFd, bmsg.c_str(), bmsg.size(), 0);
	_clients[clientFd].setBotConvStep(3);
}

void 	Server::bot3(const std::vector<std::string> &bot, const std::string &name, const int &clientFd)//client confirms bot bot bot
{
	std::string composeMsg;
	std::string bmsg;

	std::cout << "-- entering bot [3]" << std::endl;
	if (bot.size() == 1)
	{
		composeMsg = BLU + botWait + RST;
		bmsg = botMsg(name, composeMsg);
		send(clientFd, bmsg.c_str(), bmsg.size(), 0);
		_clients[clientFd].setBotConvStep(0);
	}
	else if (bot.size() > 1)
	{
		std::string msg = rebuildMessage(bot);
		int end = (int)msg.size() - 1;
		int spaces = end;
		while (msg[spaces] && std::isspace(msg[spaces]))
			--spaces;
		if (spaces != end)
			msg.erase(spaces + 1, end);
		if (msg.compare(botVerif) == 0 || msg.compare(botNameVerif) == 0)
		{
			composeMsg = PINK + botAccept + RST;
			bmsg = botMsg(name, composeMsg);
			send(clientFd, bmsg.c_str(), bmsg.size(), 0);
			composeMsg = PINK + botSerious + RST;
			bmsg = botMsg(name, composeMsg);
			send(clientFd, bmsg.c_str(), bmsg.size(), 0);
			composeMsg = PINK + botLogin + RST;
			bmsg = botMsg(name, composeMsg);
			send(clientFd, bmsg.c_str(), bmsg.size(), 0);
			composeMsg = ITALIC + BOLD + PINK + botWarning1 + RST;
			bmsg = botMsg(name, composeMsg);
			send(clientFd, bmsg.c_str(), bmsg.size(), 0);
			_clients[clientFd].setBotConvStep(4);
		}
		else
		{
			composeMsg = RDM + noBot + msg+ RST;
			bmsg = botMsg(name, composeMsg);
			send(clientFd, bmsg.c_str(), bmsg.size(), 0);
			_clients[clientFd].setBotConvStep(0);
		}
	}
}

void 	Server::bot4(const std::vector<std::string> &bot, const std::string &name, const int &clientFd)//get the login here
{
	std::string composeMsg;
	std::string bmsg;

	std::cout << "-- entering bot [4]" << std::endl;
	if (bot.size() == 1)
	{
		composeMsg = BLU + botWait + RST;
		bmsg = botMsg(name, composeMsg);
		send(clientFd, bmsg.c_str(), bmsg.size(), 0);
		_clients[clientFd].setBotConvStep(0);
	}
	else if (bot.size() > 2)
	{
		composeMsg = PINK + botWait + RST;
		bmsg = botMsg(name, composeMsg);
		send(clientFd, bmsg.c_str(), bmsg.size(), 0);
		_clients[clientFd].setBotConvStep(0);
	}
	else if (bot.size() == 2)// get API to check
	{
		return ;
	}
}

// 16/9 / 1080p / 60img/sec / use printsc / loop / son ok

void	Server::checkBot(const std::vector<std::string> &bot, int clientFd)//Lulu
{
	std::cout << "DEBUG: checkBot called with " << bot.size() << " parameters" << std::endl;
	int step = _clients[clientFd].getBotConvStep();
	std::string name = _clients[clientFd].getNickName();
	if (step == 0)
		bot0(bot, name, clientFd);
	else if (step == 1)
		bot1(name, clientFd);
	if (step == 2)
		bot2(name, clientFd);
	else if (step == 3)
		bot3(bot, name, clientFd);
	else if (step == 4)
		bot4(bot, name, clientFd);
	std::cout << "end of function checkBot()\n";
}

// PASS NICK USER JOIN TOPIC KICK MODE INFO INVITE PRIVMSG BOTde-coder

// std::map<int, std::function<void()>>
