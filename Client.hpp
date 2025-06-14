/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:15:57 by morgane           #+#    #+#             */
/*   Updated: 2025/06/14 16:11:45 by lchauffo         ###   ########.fr       */
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
		std::string	nickname;
		std::string	username;
		bool	authenticated;
		std::map<std::string, Channel *> joinedChannels;
		std::string	buffer;
		std::string	_providedPassword;

	public:
		Client(int clientSock);
		~Client();
};
