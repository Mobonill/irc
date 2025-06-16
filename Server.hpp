/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/16 19:19:43 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
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

#define SPECIAL "[]\\`^{|}"
#define ALPHANUMSPE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`^{|}-"

class Channel {};

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
	
	public:
		Server(int port, const std::string &password);
		~Server();
		void	setServerName(const std::string &newName);
		
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
		void handleCommands(int fd, const std::vector<std::string> &vectorCmd);
		// void sendError(int client_fd, const std::string &errorMsg);
		// void checkPass(int client_fd, const std::string &cmd);
		void Server::sendError(int clientFd, const std::string &code, const std::string &error);
		void checkPass(std::vector<std::string> pass, int clientFd);
		void checkNick(std::string nick, int clientFd);
};


extern Server* g_signal;