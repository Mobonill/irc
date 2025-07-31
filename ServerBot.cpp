#include "Server.hpp"
#include "BotMsg.hpp"

// Bot service interface (controlled access)
bool	Server::createBotClient(int bot_fd)
{
	if (_clients.find(bot_fd) != _clients.end())
		return false;
	_clients[bot_fd] = Client(bot_fd);
	_clients[bot_fd].setNickName(_bot->getNickName());
	_clients[bot_fd].setUserName(_bot->getUserName());
	_clients[bot_fd].setServerName(_bot->getServerName());
	_clients[bot_fd].setRealName(_bot->getRealName());
	_clients[bot_fd].setHostName(_bot->getHostName());
	_clients[bot_fd].setIp(_bot->getIp());
	_clients[bot_fd].setAuthenticated(true);
	_clients[bot_fd].setStatus(REGISTRD);
	std::cout << "Bot client created with nickname: " << _bot->getNickName() << std::endl;
	return true;
}

const std::string Server::botMsg(const std::string &cmd, const std::string &recipient, const std::string &msg)
{
	std::string full_msg;
	if (msg.empty())
		full_msg = std::string(":") + _bot->getFullAddress() + cmd + recipient + botEnd;
	else
		full_msg = std::string(":") + _bot->getFullAddress() + cmd + recipient + " :" + msg + botEnd;
	return full_msg;
}

bool	Server::addNewChannel(std::map<std::string, Channel>::iterator this_channel, const std::string& channel_name, int client_fd)
{
	//parse channel_name
	if (!(!channel_name.empty() && (channel_name[0] == '#' || channel_name[0] == '&') && channel_name.size() <= 200))
		return false;
	if (channel_name.find("^G") != std::string::npos || channel_name.find(",") != std::string::npos \
	|| channel_name.find(" ") != std::string::npos)
		return false;
	_channels.insert(std::pair<std::string, Channel>(channel_name,Channel(channel_name)));//add new channel to list of available channels
	this_channel = _channels.find(channel_name);// put the iterator to the right place
	std::map<int, Client>::iterator this_client = _clients.find(client_fd);
	this_channel->second.addClient(&(this_client->second));//add this_client to this_channel_list_clients
	this_client->second.addJoinedChannel(this_channel->first, this_channel->second);//add this_channel to this_client_joined_channel
	return true;
}

bool	Server::addBotToChannel(const std::string& channel_name, int client_fd, bool &in_channel)
{
	std::map<int, Client>::iterator bot_client = _clients.find(_bot->getBotFd());
	if (bot_client == _clients.end())//if bot isn't in all the existing clients, there is no bot to be added
		return false;
	std::map<std::string, Channel>::iterator this_channel = _channels.find(channel_name);
	if (this_channel == _channels.end())//if this channel doesn't exist, create it + shall i add the client too ?
	{
		if (addNewChannel(this_channel, channel_name, client_fd) == false)
		{
			in_channel = false;
			return false;
		}
	}
	else if (!this_channel->second.hasClient(client_fd))//if client not in this_channel, then the bot can't be added
	{
		_clients[client_fd].setBotConvStep(-1);
		return false;
	}
	else if (this_channel->second.hasClient(_bot->getBotFd()))//if already in channel, no need to add
		return true;
	this_channel->second.addClient(&(bot_client->second));//add this_client to this_channel_list_clients
	bot_client->second.addJoinedChannel(this_channel->first, this_channel->second);//add this_channel to this_client_joined_channel
	in_channel = true;
	std::string bmsg = botMsg(BOLD + " JOIN " + RST, channel_name, "");
	std::set<int> all_clients_fd;
	addClientsFd(this_channel->second, all_clients_fd);
	sendMsgListClients(std::vector<std::string>() , all_clients_fd, _bot->getBotFd(), bmsg);
	return true;
}

bool	Server::removeBotFromChannel(const std::string& channel_name)
{
	std::map<std::string, Channel>::iterator this_channel = _channels.find(channel_name);
	if (this_channel == _channels.end())
		return false;
	std::map<int, Client>::iterator bot_client = _clients.find(_bot->getBotFd());
	if (bot_client == _clients.end())
		return false;
	if (!this_channel->second.hasClient(_bot->getBotFd()))
		return false;
	this_channel->second.removeClient(bot_client->first);
	std::string bmsg = botMsg(BOLD + " PART " + RST, channel_name, "Divination session ended");
	std::set<int> all_clients_fd;
	addClientsFd(this_channel->second, all_clients_fd);
	sendMsgListClients(std::vector<std::string>(), all_clients_fd, _bot->getBotFd(), bmsg);
	return true;
}

bool	Server::isBotInChannel(const std::string& channel_name) const
{
	std::map<std::string, Channel>::const_iterator this_channel = _channels.find(channel_name);
	if (this_channel == _channels.end())
		return false;
	std::map<int, Client>::const_iterator bot_client = _clients.find(_bot->getBotFd());
	if (bot_client == _clients.end())
		return false;
	return this_channel->second.hasClient(_bot->getBotFd());
}

bool	Server::sendMessageToChannel(const std::string& channel_name, const std::string& msg)
{
	std::map<std::string, Channel>::iterator this_channel = _channels.find(channel_name);
	if (this_channel == _channels.end())
		return false;
	std::string bmsg = botMsg(BOLD + " PRIVMSG " + RST, channel_name, msg);
	std::set<int> channel_clients;
	addClientsFd(this_channel->second, channel_clients);
	sendMsgListClients(std::vector<std::string>(), channel_clients, _bot->getBotFd(), bmsg);
	return true;
}

bool	Server::sendMessageToClient(const std::string& client_nick, const std::string& msg)
{
	for (std::map<int, Client>::iterator receiver = _clients.begin(); receiver != _clients.end(); ++receiver)
	{
		if (receiver->second.getNickName() == client_nick && receiver->first != _bot->getBotFd())
		{
			std::string bmsg = botMsg(BOLD + " PRIVMSG " + RST, client_nick, msg);
			send(receiver->first, bmsg.c_str(), bmsg.size(), 0);
			return true;
		}
	}
	return false;
}

int	Server::getClientBotStep(int client_fd) const
{
	std::cout << "DEBUG: Server::getClientBotStep\n";
	std::map<int, Client>::const_iterator this_client = _clients.find(client_fd);
	std::cout << "- did it find this_client?\n";
	if (this_client != _clients.end())
	{
		std::cout << "-- yes, it did\n";
		return this_client->second.getBotConvStep();
	}
	else
		return (0);
}

void	Server::setClientBotStep(int client_fd, int step)
{
	std::map<int, Client>::iterator this_client = _clients.find(client_fd);
	if (this_client != _clients.end())
	{
		this_client->second.setBotConvStep(step);
		return ;
	}
}

void	Server::activateBot()
{
	if (_bot && !_bot->isActive())
	{
		std::cout << "Activating Bot...\n";
		_bot->createBotClient();
		std::cout << "Bot active status: " << _bot->isActive() << std::endl;
	}
}
