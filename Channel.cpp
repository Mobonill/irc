#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : _channelName(channelName) {}

Channel::~Channel() {}

const std::string &Channel::getChannelName() const { return _channelName; }

const std::set <Client> &Channel::getListClients() const { return _listClients; }

const std::map <int, std::string> &Channel::getAllClientPriviledge() const { return _clientPriviledge; }

const std::string &Channel::getClientPriviledge(int clientFd) const
{
	if (_clientPriviledge.find(clientFd) != _clientPriviledge.end())
		return _clientPriviledge.at(clientFd);
	else
		return NULL;
}

void Channel::addClient(Client *client)
{
	if (client == NULL)
		return ;
	if (_listClients.find(client) == _listClients.end())
		_listClients.insert(client);
}
