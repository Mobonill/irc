/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/07 18:04:13 by lchauffo         ###   ########.fr       */
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

		std::string _clientId;//nothing shall be kept directly on the code
		std::string _clientSecret;//same here, basic security
		std::string _redirectURI;// to see what i do there
	
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
		void parseAndExecute(int client_fd, std::string line);
		void parseCommands(int fd, const std::vector<std::string> &vectorCmd);
		void handleCommands(int fd, const std::vector<std::string> &vectorCmd);

		void sendServerMessage(int clientFd, const std::string &code, const std::string &error);
		void checkPass(std::vector<std::string> pass, int clientFd);
		void checkNick(std::vector<std::string> nick, int clientFd);
		void checkInfo(std::vector<std::string> info, int clientFd);
		void checkPrivmsg(std::vector<std::string> priv, int clientFd);
		void checkBot(std::vector<std::string> bot, int clientFd);

		template <typename T>
		const std::string &toString(T numericalValue)
		{
			std::ostringstream oss;
			oss << numericalValue;
			return oss.str();
		}
		
};

extern Server* g_signal;