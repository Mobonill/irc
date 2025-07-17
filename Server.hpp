/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/17 14:25:43 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <csignal>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <set>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "Client.hpp"
#include "Channel.hpp"
#include "Bot.hpp"

#define SPECIAL "[]\\`^{|}"
#define ALPHANUMSPE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`^{|}-"

class Channel;

#ifndef BONUS
# define BONUS 0
 class Bot;
#endif
// Parameters: <username> <hostname> <servername> <realname>
class Server
{
	private:
		std::map<int, Client> _clients;
		std::map<std::string, Channel> _channels;
		std::map<int, std::string> _clientBuffers;
		int _port;
		std::string _password;
		int _socketFd;
		bool _signal;
		int _clientsNumber;
		int _max_fd;
		std::string _serverName;
		#ifndef BONUS
		 Bot _bot;
		#endif

		void privToChannel(const std::vector<std::string> &priv, int clientFd);
		void privToClient(const std::vector<std::string> &priv, int clientFd);

		void bot0(const std::vector<std::string> &bot, const std::string &name, const int &clientFd);
		void bot1(const std::string &name, const int &clientFd);
		void bot2(const std::string &name, const int &clientFd);
		void bot3(const std::vector<std::string> &bot, const std::string &name, const int &clientFd);
		void bot4(const std::vector<std::string> &bot, const std::string &name, const int &clientFd);
	public:
		Server(int port, const std::string &password);
		~Server();
		void setServerName(const std::string &newName);
		const std::map<std::string, Channel> &getChannels() const;

		bool getSignal();
		void initServer();
		void createServer();
		void socketChecker();
		void handleNewClient();
		bool handleClientMessage(int client_fd);
		static void handleSignal(int signal);
		void closeFds();
		void stopSignal();
		void clearClient(int client_fd);
		void parseMessage(int client_fd, const std::string &msg);
		void parseCommands(int fd, const std::string &vectorCmd);
		void handleCommands(int fd, const std::vector<std::string> &vectorCmd);

		void sendMsgListClients(const std::vector<std::string> &cmd, const std::set<int> &listClients, int clientFd, std::string msg);
		const std::string privmsgMsg(int clientFd, const std::vector<std::string> &priv, const std::string &target);

		void sendServerMessage(int clientFd, const std::string &code, const std::string &error);
		// const std::string botMsg(const std::string &clientNick, const std::string &msg);

		void checkVersion(int client_fd);
		void checkStatus(int clientFd);

		void checkPass(const std::vector<std::string> &pass, int clientFd);
		void checkNick(const std::vector<std::string> &nick, int clientFd);

		void checkUser(const std::vector<std::string> &user, int fd);

		void checkInfo(const std::vector<std::string> &info, int clientFd);
		void checkPrivmsg(const std::vector<std::string> &priv, int clientFd);
		#ifndef BONUS
		void checkBot(const std::vector<std::string> &bot, int clientFd);
		#endif

		template <typename T>
		std::string toString(T numericalValue)
		{
			std::ostringstream oss;
			oss << numericalValue;
			return oss.str();
		}
		
};

extern Server* g_signal;