/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/07/31 20:38:05 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "Channel.hpp"
#include "Connexion.hpp"

class Channel;


//LULU

class Connexion;

class Client : public Connexion
{	
	private:
		std::string _buffer;
		bool _authenticated;
		bool _hasUserCommand;
		bool _hasNickCommand;
    	bool _isRegistered;

		//BOT related info
		std::string _login;
		int _bot_conv_step; //from 0 to 7 stages of tarot conv...not sure about that
		std::vector<int> _cards; // the 3 drawn numbered cards
	public:
		Client(void);
		Client(int client_socket);
		~Client();
		const std::string &getLogin() const;
		const bool &getAuthenticated() const;
		const int &getBotConvStep() const;

		void setAuthenticated(const bool &status);
		void setNickName(const std::string &new_nickname);
		void setUserName(const std::string &new_username);
		void setServerName(const std::string &new_servername);
		void setHostName(const std::string &new_hostname);
		void setRealName(const std::string &new_realname);
		void setIp(const std::string &ip);
		void setBotConvStep(const int &updated_step);
		void setLogin(const std::string &new_login);
		void setStatus(const int &new_status);
		
		bool hasUserCommand() const;
		bool hasNickCommand() const;
		bool isRegistered() const;
		void setHasUserCommand(bool val);
		void setHasNickCommand(bool val);
		void setRegistered(bool val);

};
