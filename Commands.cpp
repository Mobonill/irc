/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:36:07 by lchauffo          #+#    #+#             */
/*   Updated: 2025/06/27 17:44:26 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.cpp"
#include "UtilsCmds.cpp"
#include <cctype>
#include <fstream>
#include <set>

//:servername CODE * COMMAND :message
//:servername → your server name
// CODE → IRC numeric reply
// * → the client nickname, if not registered put '*' instead
// COMMAND → the command being responded to
// :message → the error message

void	Server::sendServerMessage(int clientFd, const std::string &code, const std::string &entry)
{
	std::string clientName = _clients[clientFd].getNickName();
	std::string fullMessage = ":" + _serverName + " " + code + " " + clientName + " " + entry + "\r\n";
	send(clientFd, fullMessage.c_str(), fullMessage.size(), 0);
}

void	Server::checkPass(std::vector<std::string> pass, int clientFd)
{
	if (pass.size() < 2)
		return sendServerMessage(clientFd, "461", "PASS :Not enough parameters");
	else if (pass.size() < 2)
		return sendServerMessage(clientFd, "461", "PASS :Too many parameters");
	else if (_clients[clientFd].getAuthenticated() == true)
		return sendServerMessage(clientFd, "462", "PASS :You may not reregister");
	// else if (pass[1].size() > 128) //OWASP Authentication Cheat Sheet, can be down to 64 chars
	// 	return sendServerMessage(clientFd, "464", "PASS :Password incorrect");
	else if (pass[1].compare(_password) == false)
		return sendServerMessage(clientFd, "464", "PASS :Password incorrect");
	else
	{
		if (pass[1].compare(_password) == false)
			return sendServerMessage(clientFd, "464", "PASS :Password incorrect");
		_clients[clientFd].setAuthenticated(true);
		std::cout << "Client " << clientFd << " authenticated successfully." << std::endl;
	}
	return ;
}

void	Server::checkNick(std::vector<std::string> nick, int clientFd)
{
	if (nick.empty())
		return sendServerMessage(clientFd, "432", "NICK :Erroneus nickname");
	else if (_clients[clientFd].getAuthenticated() == false)
		return ;
	else if ((_clients[clientFd].getNickName() == "*" && nick.size() != 3) || nick.size() != 2)
		return sendServerMessage(clientFd, "431", "NICK :No nickname given");
	else
	{
		if (nick.size() > 9 || (!std::isalpha(nick[1][0]) && !isValid(nick[1][0], SPECIAL))
		|| !onlyValid(&nick[1][1], ALPHANUMSPE))
			return sendServerMessage(clientFd, "432", "NICK :Erroneus nickname");
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			if (it->second.getNickName().compare(nick[1]) == true)
				return sendServerMessage(clientFd, "433", "NICK :Nickname is already in use");
		std::string oldNick = _clients[clientFd].getNickName();
		_clients[clientFd].setNickName(nick[1]);
		if (oldNick == "*")
			std::cout << "Introducing new nick \"" << _clients[clientFd].getNickName() << "\"." << std::endl;
		else
		{
			std::string msg = ":" + oldNick + " NICK " + _clients[clientFd].getNickName() + "\r\n";
			std::set<int> sameChannelsClients;
			for (std::map<std::string, Channel> ::const_iterator mit = _clients[clientFd].getJoinedChannels().begin();
			mit != _clients[clientFd].getJoinedChannels().end(); ++mit)
				addClientsFd(mit->second, sameChannelsClients);
			sendMsgListClients(sameChannelsClients, clientFd, msg);
			return ;
		}
	}
}

// input a name to make a guest an actual client
// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
void	checkUser(int fd, std::string cmd)//Morgane
{}

// JOIN #[name of channel here]
// can't start any discussion if no channel created, even a general one
// no user == no channel, only at the start of a server
void	checkJoin(int fd, std::string cmd)//Zara
{}

void	checkTopic(int fd, std::string cmd)//Zara
{}

void	checkKick(int fd, std::string cmd)//Zara
{}

void	checkMode(int fd, std::string cmd)//Zara
{}

void	Server::checkInfo(std::vector<std::string> info, int clientFd)//Lulu
{
	if (info.size() >= 2 && info[1] != _serverName)
		return sendServerMessage(clientFd, "402", info[1] + " :No such server");//to be verified
	sendServerMessage(clientFd, "373", ":INFO start");
	sendServerMessage(clientFd, "371", ":Server name: " + _serverName);
	sendServerMessage(clientFd, "371", ":Port: " + toString(_port));
	sendServerMessage(clientFd, "371", ":Compiled on: " + __DATE__ " " + __TIME__);
	sendServerMessage(clientFd, "374", ":End of /INFO list");
}

void	checkInvite(int fd, std::string cmd)//Zara
{}

// Each IRC message may consist of up to three main parts: the prefix
// (OPTIONAL), the command, and the command parameters (maximum of
// fifteen (15)).  The prefix, command, and all parameters are separated
// by one ASCII space character (0x20) each.

void	privToChannel(std::vector<std::string> priv, int clientFd, std::map<std::string, Channel> serverChannels, std::map<int, Client> serverClients)
{
	if (priv[2][0] != ':')
		return sendServerMessage(clientFd, "412", "PRIVMSG :No text to send");
	std::map<std_string, Channel>::iterator thisChannel = serverchannels.find(priv[1]);
	if (thisChannel != serverchannels.end())
	{
		std::string msg = privmsgMsg(serverclients[clientFd], priv);
		std::set<int> thisChannelClients;
		addClientsFd(thisChannel->second, thisChannelClients);
		sendMsgListClients(thisChannelClients,clientFd, msg);
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
				for (std::map<std::string, Channel>::iterator mit = serverchannels.begin();
				mit != serverchannels.end(); ++mit)
					if (wildcardMatch(mit->first, priv[1]) == true && ++findMatchChannel)
						if (mit->second.getClientPriviledge(clientFd).find('@') != std::string::npos && ++operatorStatus)
							addClientsFd(mit->second, allMatchChannelsClients);
				sendMsgListClients(allMatchChannelsClients,clientFd, msg);
			}
		}
		if (findMatchChannel == 0)
			return sendServerMessage(clientFd, "401", "PRIVMSG"+ priv[1] + ":No such nick/channel");
		if (operatorStatus == 0)
			return sendServerMessage(clientFd, "481", "PRIVMSG"+ priv[1] + ":Permission Denied- You're not an IRC operator");
	}
}

void	privToClient(std::vector<std::string> priv, int clientFd, std::map<int, Client> serverClients)
{
	std::vector<std::string> multiClients = splitString(priv[1], ',');
	if (multiClients.size() > 15)
		return sendServerMessage(clientFd, "407", "PRIVMSG"+ priv[1] + ":Duplicate recipients. No message delivered");
	std::set<int> existingClients;
	std::set<std::string> notFound;
	std::string msg = privmsgMsg(serverClients[clientFd], priv);
	for (std::vector<std::string>::iterator thisClient = multiClients.begin(); thisClient != multiClients.end(); ++thisClient)
	{
		for (std::map <int, Client>::iterator it = serverClients.begin(); it != serverClients.end(); ++it)
			if (thisClient == it->second.getNickName())
				existingClients.insert(it->first);
			else
				notFound.insert(thisClient);
		sendMsgListClients(existingClients, clientFd, msg);
	}
	for (std::set<std::string>::iterator sit = notFound.begin(); sit != notFound.end(); ++sit)
		sendServerMessage(clientFd, "401", "PRIVMSG"+ *sit + ":No such nick/channel");
}

// :nickname!username@hostname
void	Server::checkPrivmsg(std::vector<std::string> priv, int clientFd)//Lulu
{
	if (priv.size() == 1)
		return sendServerMessage(clientFd, "411", "PRIVMSG :No recipient given");
	else 
	{
		if (priv[1][0] == ':')
			return sendServerMessage(clientFd, "411", "PRIVMSG :No recipient given");
		else if (priv.size() == 2)
			return sendServerMessage(clientFd, "412", "PRIVMSG :No text to send");
		if (priv[1][0] == '#')
			privToChannel(priv, clientFd, _channels, _clients);
		else
			privToClient(priv, clientFd, _clients);
	}
}
// PRIVMSG Bot :BOT --> do i need to make a botFd ?? == create/reset clientSession



bool summonBot(const std::string &msg)
{
	std::set<std::std::string> summonBot = {"divination", "do divination", "tarots", \
	"my futur", "42forecast", "DE-CODER", "Dcoder", "decoder"};
	if (summonBot.find(msg) != summonBot.end())
		return true;
	return false;
}
#define bot0 ""
void	Server::checkBot(std::vector<std::string> bot, int clientFd)//Lulu
{
	if (bot.size() > 2 || (bot.size() == 2 && summonBot(rebuildMessage(bot)) == false))
		return sendServerMessage(clientFd, "421", "BOT :Unknown command");
	std::string bmsg = botMsg(_clients[clientFd].getNickName(), "Welcome ");
	send(clientFd, bmsg.c_str(), bmsg.size(), 0);
	
}

// PASS NICK USER JOIN TOPIC KICK MODE INFO INVITE PRIVMSG BOTde-coder