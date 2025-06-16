/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/16 20:22:37 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientSock) : _clientSocket(clientSock), _nickname("*") {}

Client::~Client() {}

int Client::getClientSocket() const { return _clientSocket; }

const std::string &Client::getNickName() const { return _nickname; }

const bool &Client::getAuthenticated() const { return _authenticated; }

const std::map<std::string, Channel> &Client::getJoinedChannels() const { return _joinedChannels; }

void Client::setAuthenticated(const bool &status) { _authenticated = status; }

void Client::setNickName(const std::string &newNickName) { _nickname = newNickName; }
