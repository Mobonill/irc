/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/23 19:40:14 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientSock) : _clientSocket(clientSock), _nickname("*"), _status(NOT_AUTHENTCATD) {}

Client::~Client() {}

int Client::getClientSocket() const { return _clientSocket; }

const std::string &Client::getNickName() const { return _nickname; }

const std::string &Client::getUserName() const { return _username; }

const std::string &Client::getIp() const { return _ip; }

const bool &Client::getAuthenticated() const { return _authenticated; }

const std::map<std::string, Channel> &Client::getJoinedChannels() const { return _joinedChannels; }

const int &Client::getStatus() const { return _status; }

void Client::setAuthenticated(const bool &status) { _authenticated = status; }

void Client::setNickName(const std::string &newNickName) { _nickname = newNickName; }

void Client::setUserName(const std::string &newUserName) { _username = newUserName;}

void Client::setIp(const std::string &ip)
{
    if (_ip.empty())
        _ip = ip;
    else
        return ;
}

