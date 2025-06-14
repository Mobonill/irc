/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:36:07 by lchauffo          #+#    #+#             */
/*   Updated: 2025/06/14 16:26:14 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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

void	checkPass(int fd, std::string cmd)
{
	if ()
	//check password policy
	//:localhost 461 * PASS :Not enough parameters
	//:localhost 461 * PASS :Too many parameters
	//:server 464 * :Password incorrect
	return ;
}
// if NICK or USER, before PASS == :localhost 464 * :Password required
void	checkNick(int fd, std::string cmd)
{}

void	checkUser(int fd, std::string cmd)
{}

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
