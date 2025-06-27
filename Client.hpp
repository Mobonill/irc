/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/06/27 11:23:11 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include "Channel.hpp"

class Channel;

class Client
{	
private:
	int _clientSocket;
	std::string	_nickname;
	std::string	_username;
	bool _authenticated; //это логическая (булевая) переменная, которая показывает, прошёл ли клиент аутентификацию (то есть, зарегистрировался ли на сервере IRC).
	std::map<std::string, Channel*> _joinedChannels; // указатели на каналы
	std::string	_buffer;
	std::string	_providedPassword;
	std::string _ipAddress;

public:
	Client(); // добавлен конструктор по умолчанию
	Client(int clientSock);
	~Client();

	int getClientSocket() const;
	const std::string &getNickName() const;
	bool getAuthenticated() const;
	const std::map<std::string, Channel*> &getJoinedChannels() const;

	void setAuthenticated(bool status);
	void setNickName(const std::string &newNickName);

	const std::string &getBuffer() const;
	void setBuffer(const std::string &buffer);

	const std::string &getProvidedPassword() const;
	void setProvidedPassword(const std::string &password);

	const std::string &getIpAddress() const;
	void setIpAddress(const std::string &ip);
};
