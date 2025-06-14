/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:20:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/14 20:26:23 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientSock) : _clientSocket(clientSock), _nickname("*") {}

Client::~Client() {}

int Client::getClientSocket() const { return this->_clientSocket; }

const std::string &Client::getNickName() const { return this->_nickname; }

const bool &Client::getAuthenticated() const { return this->_authenticated; }

void Client::setAuthenticated(const bool &status) { _authenticated = status; }
