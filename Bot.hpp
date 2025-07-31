#pragma once

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "BotMsg.hpp"
#include "Connexion.hpp"
#include "Server.hpp"
#include "Style.hpp"

class Server;
class Client;

class Bot : public Connexion
{
	private:
		std::string _client_id;//nothing shall be kept directly on the code
		std::string _client_secret;//same here, basic security
		std::string _redirect_uri;// to see what i do there
		std::string _access_token;

		static const int BOT_FD = -1;
		bool _isactive;

		Server* _server; // Reference to server for accessing channels/clients
		void loadOAuthConfig();
		void refreshAccessToken();
	public:
		Bot();
		Bot(Server *server);
		~Bot();


		void createBotClient();
		void joinChannel(const std::string &channel_name, bool &in_channel);
		void leaveChannel(const std::string &channel_name);
		void sendMessage(const std::string &target, const std::string &msg, bool is_channel);

		bool isActive() const;
		int getBotFd() const;

		// Bot conversation methods
		void handleConversation(int client_fd, const std::string &msg,\
		const std::string &client_nick, const std::string & channel_name, bool in_channel);
		void echoClientMessage(std::string &msg, const std::string &full_client_name, int client_fd);

		void stepIllegalCommand(const int &client_fd, const std::string &nick);
		void stepUninvitedChannel(const int &client_fd, const std::string &nick, const std::string &illegal_channel);
		void step0(const int &client_fd, const std::string &msg, const std::string &nick, \
		const std::string &target, bool in_channel);
		void step1(const int &client_fd, const std::string &nick, const std::string &target, \
		bool in_channel);
		void step2(const int &client_fd, const std::string &nick, const std::string &target, \
		bool in_channel);
		void step3(const int &client_fd, std::string msg, const std::string &nick, \
		const std::string &target, bool in_channel);
		void step4(const int &client_fd, std::string login, const std::string &nick, \
		const std::string &target, bool in_channel);
};
