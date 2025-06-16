#pragma once
#include <string>
#include <map>
#include <vector>

#include "Client.hpp"

// _ClientPriviledge
// managing operator privileges, which allow certain users to kick 
// or ban others, set topics, and tweak channel settings

// tracking which users are in the room at any given time
class Channel
{
	protected:
		Channel();
	private:
		std::string _channelName;
		int _userLimits;
		std::string _topic;
		std::string _password;
		// std::map<Client, std::string> _ClientPriviledge;
		std::map <int, std::string> _clientPriviledge;
		std::vector <Client> _listClients;
	public:
		Channel(const std::string &channelName);
		~Channel();
		const std::vector <Client> &getListClients() const;
		const std::map <int, std::string> &getAllClientPriviledge() const; // see if needed
		const Client &getClientPriviledge(int clientFd) const;
};