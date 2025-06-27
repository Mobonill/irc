/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/24 11:41:54 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Client.hpp"

// Конструктор по умолчанию
Client::Client()
  : _clientSocket(-1), _nickname("*"), _authenticated(false) {}

Client::Client(int clientSock) 
  : _clientSocket(clientSock), _nickname("*"), _authenticated(false) {}

Client::~Client() {}

int Client::getClientSocket() const { return _clientSocket; }

const std::string &Client::getNickName() const { return _nickname; }

bool Client::getAuthenticated() const { return _authenticated; }

const std::map<std::string, Channel*> &Client::getJoinedChannels() const {
    return _joinedChannels;
}

void Client::setAuthenticated(bool status) { _authenticated = status; }

void Client::setNickName(const std::string &newNickName) { _nickname = newNickName; }

// Реализация геттеров и сеттеров для buffer
const std::string &Client::getBuffer() const { return _buffer; }
void Client::setBuffer(const std::string &buffer) { _buffer = buffer; }

// Реализация геттеров и сеттеров для providedPassword
const std::string &Client::getProvidedPassword() const { return _providedPassword; }
void Client::setProvidedPassword(const std::string &password) { _providedPassword = password; }

// Реализация геттеров и сеттеров для ipAddress
const std::string &Client::getIpAddress() const { return _ipAddress; }
void Client::setIpAddress(const std::string &ip) { _ipAddress = ip; }
