/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/15 13:21:25 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <cerrno>
#include <sstream>
#include "Client.hpp"



class Channel;

class Server {
    
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
    
    public:
		Server(int port, const std::string &password);
		~Server();
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
		void sendError(int client_fd, const std::string &errorMsg);
		void checkPass(int client_fd, const std::string &cmd);
		void checkNick(int client_fd, const std::string &cmd);
		    // ❗ Добавь эту строку:
    bool isAuthenticated(int fd);

    // остальные методы
   // void checkJoin(int fd, const std::string &cmd);
    void checkInvite(int fd, const std::string &cmd);
    void checkTopic(int fd, const std::string &cmd);

	int getClientFdByNick(const std::string& nick);
	

	void checkJoin(int fd, const std::string &cmd);
	void handleJoinZero(int fd);
	void handleJoinChannels(int fd, const std::vector<std::string> &tokens);
	Channel& getOrCreateChannel(const std::string &channelName, int fd);
	void handleSingleChannelJoin(int fd, Client &client, const std::string &channelName, const std::string &providedKey);
	void sendJoinMessages(int fd, Client &client, Channel &channel);

	void checkKick(int fd, const std::string &cmd);
	Channel* getChannelOrSendError(int fd, const std::string& channelName);
    int findClientFdByNick(const std::string& nick) const;
    Client* getClientOrSendError(int fd);
    void executeKick(Channel& channel, Client& kicker, int targetFd, const std::string& targetNick, const std::string& channelName);
    

	void checkMode(int fd, const std::string &cmd);
    bool processModeChange(int fd, const std::vector<std::string> &tokens, Channel &channel, Client &client, const std::string &target);
    void sendCurrentModes(int fd, Client &client, Channel &channel, const std::string &target);
	bool isValidChannelName1(const std::string& channelName);

	void handlePrivmsg(int senderFd, const std::vector<std::string>& tokens);
	
};


extern Server* g_signal;