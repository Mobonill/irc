#pragma once
#include <string>
#include <map>
#include <set>

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
		std::string _channel_name;
		int _user_limits;
		std::string _topic;
		std::string _password;
		std::map <int, std::string> _client_priviledge;
		std::set <Client *> _list_clients;
	public:
		Channel(const std::string &channel_name);
		~Channel();
		const std::string &getChannelName() const;
		const std::set <Client *> &getListClients() const;
		const std::map <int, std::string> &getAllClientPriviledge() const; // see if needed
		const std::string getClientPriviledge(int client_fd) const;
		void addClient(Client *client);
		bool hasClient(int client_fd) const;
		bool removeClient(int bannished_fd);
};
