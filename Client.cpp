/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/23 16:35:09 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : Connexion(), _bot_conv_step(0)
{
	_nickname = "*";
}

Client::Client(int client_socket) : Connexion(), _bot_conv_step(0)
{
	_nickname = "*";
	_socket = client_socket;
}

Client::~Client() {}


const std::string &Client::getLogin() const { return _login; }

const bool &Client::getAuthenticated() const { return _authenticated; }

const int &Client::getBotConvStep() const { return _bot_conv_step; }


void Client::setAuthenticated(const bool &status) { _authenticated = status; }

void Client::setNickName(const std::string &new_nickname) { _nickname = new_nickname; }

void Client::setUserName(const std::string &new_username) { _username = new_username;}

void Client::setServerName(const std::string &new_servername) { _servername = new_servername;}

void Client::setHostName(const std::string &new_hostname) { _hostname = new_hostname;}

void Client::setRealName(const std::string &new_realname) { _realname = new_realname;}

void Client::setIp(const std::string &ip) { if (_ip.empty()) _ip = ip; }

void Client::setLogin(const std::string &newLogin) { if (_login.empty()) _login = newLogin; }

void Client::setBotConvStep(const int &updated_step)
{
	if (updated_step >= 0 && updated_step <= 7)
		_bot_conv_step = updated_step;
	if (updated_step == -1)
		_bot_conv_step = (_bot_conv_step + 1) * (-1);
}

void Client::setStatus(const int &new_status) { if (new_status >= NOT_AUTHENTCATD && new_status <= IN_CHANNEL) _status = new_status; }
