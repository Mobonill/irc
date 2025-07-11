#pragma once
#include <string>
#include <map>
#include <set>

#include "Client.hpp"

class Client;

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
		std::map <int, std::string> _clientPriviledge;
		std::set <Client *> _listClients;
	public:
		Channel(const std::string &channelName);
		~Channel();
		const std::string &getChannelName() const;
		const std::set <Client *> &getListClients() const;
		const std::map <int, std::string> &getAllClientPriviledge() const; // see if needed
		const std::string getClientPriviledge(int clientFd) const;
		void addClient(Client *client);
};
