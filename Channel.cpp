#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(const std::string &channel_name) : _channel_name(channel_name) {}

Channel::~Channel() {}

const std::string &Channel::getChannelName() const { return _channel_name; }

const std::set <Client *> &Channel::getListClients() const { return _list_clients; }

const std::map <int, std::string> &Channel::getAllClientPriviledge() const { return _client_priviledge; }

const std::string Channel::getClientPriviledge(int clientFd) const
{
	std::string nullstr = "";
	if (_client_priviledge.find(clientFd) != _client_priviledge.end())
		return _client_priviledge.at(clientFd);
	else
		return nullstr;
}

void Channel::addClient(Client *client)
{
	if (client == NULL)
		return ;
	if (_list_clients.find(client) == _list_clients.end())
		_list_clients.insert(client);
}

bool Channel::hasClient(int client_fd) const
{
	std::set<Client *>::iterator this_client;
	for (this_client = _list_clients.begin(); this_client != _list_clients.end(); ++this_client)
		if ((*this_client)->getSocket() == client_fd)
			return true;
	return false;
}

bool Channel::removeClient(int bannished_fd)
{
	std::set<Client *>::iterator bannished;
	for (bannished = _list_clients.begin(); bannished != _list_clients.end(); ++bannished)
		if ((*bannished)->getSocket() == bannished_fd)
			break;
	if (bannished == _list_clients.end())
		return false;
	_client_priviledge.erase(bannished_fd);
	_list_clients.erase(bannished);
	return true;
}