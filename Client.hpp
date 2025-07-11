/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/07/10 15:00:28 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>

#include "Channel.hpp"

class Channel;

enum stateMachine
{
	NOT_AUTHENTCATD,
	NOT_REGISTRD,
	REGISTRD,
	IN_CHANNEL
};

class Client
{	
	private:
		int _clientSocket;
		std::string	_nickname;

		std::string _servername;
		std::string _username;
		std::string _hostname;
		std::string _realname;

		std::string	_ip;
		bool _authenticated;
		std::map<std::string, Channel> _joinedChannels;
		std::string	_buffer;
		int	_status;
		
		//BOT related info
		std::string _login;
		int _botConvStep; //from 0 to 7 stages of tarot conv...not sure about that
		std::vector<int> _cards; // the 3 drawn numbered cards
	public:
		Client(void);
		Client(int clientSock);
		~Client();
		int getClientSocket() const;
		const std::string &getNickName() const;
		const std::string &getUserName() const;
		const std::string &getIp() const;
		const std::string &getLogin() const;
		const bool &getAuthenticated() const;
		const std::map<std::string, Channel> &getJoinedChannels() const;
		const int &getStatus() const;
		const int &getBotConvStep() const;

		void setAuthenticated(const bool &status);
		void setNickName(const std::string &newNickName);
		void setUserName(const std::string &newUserName);
		void setServerName(const std::string &newServerName);
		void setHostName(const std::string &newHostName);
		void setRealName(const std::string &newRealName);
		void setIp(const std::string &ip);
		void setBotConvStep(const int &updatedStep);
		void setLogin(const std::string &newLogin);
		void setStatus(const int &newStatus);
};
