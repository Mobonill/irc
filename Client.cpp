/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <zserobia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/07/31 14:26:35 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Client.hpp"

// Конструктор по умолчанию
Client::Client()
  : _clientSocket(-1),
    _nickname("*"),
    _authenticated(false),
    _status(NOT_AUTHENTCATD),
    _botConvStep(0),
    _hasUserCommand(false),
    _hasNickCommand(false),
    _isRegistered(false)
{}


Client::Client(int clientSock)
  : _clientSocket(clientSock),
    _nickname("*"),
    _authenticated(false),
    _status(NOT_AUTHENTCATD),
    _botConvStep(0),
    _hasUserCommand(false),
    _hasNickCommand(false),
    _isRegistered(false)
{}



Client::~Client() {}

int Client::getClientSocket() const { return _clientSocket; }

const std::string &Client::getNickName() const { return _nickname; }

//bool Client::getAuthenticated() const { return _authenticated; }

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

//Client::Client(void) : _clientSocket(0), _nickname("*"), _status(NOT_AUTHENTCATD), _botConvStep(0) {}
//Client::Client(int clientSock) : _clientSocket(clientSock), _nickname("*"), _status(NOT_AUTHENTCATD), _botConvStep(0) {}

//Client::~Client() {}


std::string summonExpressions = "divination,tarot,futur,42forecast,DE-CODER,Dcoder,decoder,read me,akinator";
//const std::string &Client::getNickName() const { return _nickname; }

const std::string &Client::getUserName() const { return _username; }

const std::string &Client::getIp() const { return _ip; }

const std::string &Client::getLogin() const { return _login; }
const std::string &Client::getHostName() const {
    return _hostname;
}
const bool &Client::getAuthenticated() const { return _authenticated; }

//const std::map<std::string, Channel> &Client::getJoinedChannels() const { return _joinedChannels; }

const int &Client::getStatus() const { return _status; }

const int &Client::getBotConvStep() const { return _botConvStep; }

void Client::setUserName(const std::string &newUserName) { _username = newUserName;}
void Client::setServerName(const std::string &newServerName) { _servername = newServerName; }
void Client::setHostName(const std::string &newHostName) { _hostname = newHostName; }
void Client::setRealName(const std::string &newRealName) { _realname = newRealName; }


void Client::setIp(const std::string &ip)
{
    if (_ip.empty())
        _ip = ip;
}

void Client::setLogin(const std::string &newLogin)
{
    if (_login.empty())
        _login = newLogin;
}

void Client::setBotConvStep(const int &updatedStep)
{
    if (updatedStep >= 0 && updatedStep <= 7)
        _botConvStep = updatedStep;
}

void Client::setStatus(const int &newStatus)
{
    if (newStatus >= NOT_AUTHENTCATD && newStatus <= IN_CHANNEL)
        _status = newStatus;
}

		bool Client::hasUserCommand() const { return _hasUserCommand; }
		void Client::setHasUserCommand(bool val) { _hasUserCommand = val; }
		bool Client::hasNickCommand() const { return _hasNickCommand; }
		void Client::setHasNickCommand(bool val) { _hasNickCommand = val; }
		bool Client::isRegistered() const { return _isRegistered; }
		void Client::setRegistered(bool val) { _isRegistered = val; }