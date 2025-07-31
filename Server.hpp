/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/31 20:25:15 by zserobia         ###   ########.fr       */
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
#include <fstream>
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
#include "Utils.hpp"
#include "Style.hpp"

#define SPECIAL "[]\\`^{|}"
#define ALPHANUMSPE "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`^{|}-"
#define END "\r\n"

class Channel;
class Client;//

#ifndef BONUS
# define BONUS 0
#endif

#if BONUS
 #include "Bot.hpp"
 class Bot;
#endif

class Server
{
	private:
		std::map<int, Client> _clients;
		std::map<std::string, Channel> _channels;
		std::map<int, std::string> _client_buffers;
		int _port;
		std::string _password;
		int _socket_fd;
		bool _signal;
		int _clients_number;
		int _max_fd;
		std::time_t _creationTime;
		std::string _server_name;
		#if BONUS
		 Bot *_bot;
		#endif

		void privToChannel(const std::vector<std::string> &priv, int client_fd);
		void privToClient(const std::vector<std::string> &priv, int client_fd);

	public:
		Server(int port, const std::string &password);
		~Server();
		void setServerName(const std::string &new_name);
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
    std::string getServerCreationDate();

	void checkMode(int fd, const std::string &cmd);
    bool processModeChange(int fd, const std::vector<std::string> &tokens, Channel &channel, Client &client, const std::string &target);
    void sendCurrentModes(int fd, Client &client, Channel &channel, const std::string &target);
	bool isValidChannelName1(const std::string& channelName);

	void handlePrivmsg(int senderFd, const std::vector<std::string>& tokens);
	void checkUser(int fd, const std::string& cmd);
	void sendWelcomeMessage(Client& client);
	void sendToClient(int fd, const std::string& message);

	//LULU under
	
		void parseCommands(int fd, const std::string &vector_cmd);
		void handleCommands(int fd, const std::vector<std::string> &vector_cmd);

		void sendMsgListClients(const std::vector<std::string> &cmd, const std::set<int> &list_clients, int client_fd, std::string msg);
		const std::string privmsgMsg(int client_fd, const std::vector<std::string> &priv, const std::string &target);

		void sendServerMessage(int client_fd, const std::string &code, const std::string &error);

		void checkVersion();
		void checkStatus(int client_fd);

		void checkPass(const std::vector<std::string> &pass, int client_fd);
		void checkNick(const std::vector<std::string> &nick, int client_fd);

		void checkUser(const std::vector<std::string> &user, int fd);

		void checkInfo(const std::vector<std::string> &info, int client_fd);
		void checkPrivmsg(const std::vector<std::string> &priv, int client_fd);

		void checkBot(const std::vector<std::string> &bot, int client_fd);
		void checkServer(const std::vector<std::string> &serv, int client_fd);
		void checkColor(int client_fd);

		template <typename T>
		std::string toString(T numerical_value)
		{
			std::ostringstream oss;
			oss << numerical_value;
			return oss.str();
		}

		bool	addNewChannel(std::map<std::string, Channel>::iterator this_channel, \
		const std::string& channel_name, int client_fd);

		// Bot service interface (controlled access)
		#if BONUS
		 bool createBotClient(int bot_fd);
		 bool addBotToChannel(const std::string& channel_name, int client_fd, bool &in_channel);
		 bool removeBotFromChannel(const std::string& channel_name);
		 bool isBotInChannel(const std::string& channel_name) const;
		 const std::string botMsg(const std::string &cmd, const std::string &recipient, const std::string &msg);
		 bool sendMessageToChannel(const std::string& channel_name, const std::string& msg);
		 bool sendMessageToClient(const std::string& client_nick, const std::string& msg);
		 int getClientBotStep(int client_fd) const;
		 void setClientBotStep(int client_fd, int step);
		 void activateBot();
		#endif
};

extern Server* g_signal;