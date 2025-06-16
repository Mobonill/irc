#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : _channelName(channelName) {}

Channel::~Channel() {}

const std::vector <Client> &Channel::getListClients() const { return _listClients; }

const std::map <int, std::string> &Channel::getAllClientPriviledge() const { return _clientPriviledge; }

const Client &Channel::getClientPriviledge(int clientFd) const { return _clientPriviledge[clientFd]; }
