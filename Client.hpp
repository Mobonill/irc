/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/06/14 20:25:07 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>

class Channel;

class Client
{	
	private:
		int _clientSocket;
		std::string	_nickname;
		std::string	_username;
		bool _authenticated;//if authenticated no more check pass
		std::map<std::string, Channel *> _joinedChannels;
		std::string	_buffer;
		std::string	_providedPassword;
		//std::string ipAddress;?? do i need it and where, if yes, do i need setter?

	public:
		Client(int clientSock);
		~Client();
		int getClientSocket() const;
		const std::string &getNickName() const;
		const bool &getAuthenticated() const;
		void setAuthenticated(const bool &status);
};
