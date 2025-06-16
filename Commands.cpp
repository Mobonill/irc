/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:36:07 by lchauffo          #+#    #+#             */
/*   Updated: 2025/06/16 20:37:28 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.cpp"
#include "cctype"

// void Server::checkPass(int client_fd, const std::string &cmd)
// {
//     std::vector<std::string> pass = splitString(cmd, " ");
//   (void)client_fd;
//     if (pass.size() != 2)
//     {
//         // sendError(client_fd, "461 PASS :Not enough parameters");
//         return;
//     }
//     if (pass[1] == _password) 
//         std::cout << "Client " << client_fd << " authenticated successfully." << std::endl;
//     else {
//         // sendError(client_fd, "464 :Password incorrect");
//         clearClient(client_fd);
//     }
// }

// void Server::sendError(int client_fd, const std::string &errorMsg)
// {
//    // std::string errorMessage = ":" + _serverName + " " + errorMsg + "\r\n";
//     send(client_fd, errorMessage.c_str(), errorMessage.size(), 0);
// }

//:servername CODE * COMMAND :message
//:servername → your server name
// CODE → IRC numeric reply
// * → the client nickname, if not registered put '*' instead
// COMMAND → the command being responded to
// :message → the error message


void	Server::sendError(int clientFd, const std::string &code, const std::string &error)
{
	std::string clientName = _clients[clientFd].getNickName();
	std::string errMessage = ":" + _serverName + " " + code + " " + clientName + " " + error + "\r\n";
	send(clientFd, errMessage.c_str(), errMessage.size(), 0);
}

void	Server::checkPass(std::vector<std::string> pass, int clientFd)
{
	if (pass.size() < 2)
		return sendError(clientFd, "461", "PASS :Not enough parameters");
	else if (pass.size() < 2)
		return sendError(clientFd, "461", "PASS :Too many parameters");
	else if (_clients[clientFd].getAuthenticated() == true)
		return sendError(clientFd, "462", "PASS :You may not reregister");
	// else if (pass[1].size() > 128) //OWASP Authentication Cheat Sheet, can be down to 64 chars
	// 	return sendError(clientFd, "464", "PASS :Password incorrect");
	else if (pass[1].compare(_password) == false)
		return sendError(clientFd, "464", "PASS :Password incorrect");
	else
	{
		if (pass[1].compare(_password) == false)
			return sendError(clientFd, "464", "PASS :Password incorrect");
		_clients[clientFd].setAuthenticated(true);
		std::cout << "Client " << clientFd << " authenticated successfully." << std::endl;
	}
	return ;
}
// Example:

// NICK Wiz                        ; Introducing new nick "Wiz".
// :WiZ NICK Kilroy                ; WiZ changed his nickname to Kilroy.
// [serverName]:oldnick NICK newnick
void	Server::checkNick(std::string nick, int clientFd)
{
	if (nick.empty())
		return sendError(clientFd, "432", "NICK :Erroneus nickname");
	else if (_clients[clientFd].getAuthenticated() == false)
		return sendError(clientFd, "464", "PASS :Password required");//non-standard error, following RFC logics, else can just ignore
	else if (nick.size() != 2)
		return sendError(clientFd, "431", "NICK :No nickname given");
	else
	{
		if (nick.size() > 9 || (!std::isalpha(nick[0]) && !isValid(nick[0], SPECIAL))
		|| !onlyValid(&nick[1], ALPHANUMSPE))
			return sendError(clientFd, "432", "NICK :Erroneus nickname");
		for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second.getNickName().compare(nick) == true)
				return sendError(clientFd, "433", "NICK :Nickname is already in use");
		}
		std::string oldNick = _clients[clientFd].getNickName();
		_clients[clientFd].setNickName(nick);
		if (oldNick == "*")
			std::cout << "Introducing new nick \"" << _clients[clientFd].getNickName() << "\"." << std::endl;
		else
		{
			for (std::map<std::string, Channel> ::const_iterator it = _clients[clientFd].getJoinedChannels().begin();
			it != _clients[clientFd].getJoinedChannels().end(); it++)
			_clients[clientFd];//get all channels fd in order to printf_fd and announce change nickname
		}
	}
}
// if NICK or USER, before PASS == :localhost 464 * :Password required

// input a name to make a guest an actual client
// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
void	checkUser(int fd, std::string cmd)
{
	
}

void	checkJoin(int fd, std::string cmd)
{}

void	checkTopic(int fd, std::string cmd)
{}

void	checkKick(int fd, std::string cmd)
{}

void	checkMode(int fd, std::string cmd)
{}

void	checkInfo(int fd, std::string cmd)
{}

void	checkInvite(int fd, std::string cmd)
{}

void	checkPrivmsg(int fd, std::string cmd)
{}

void	checkBot(int fd, std::string cmd)
{}
