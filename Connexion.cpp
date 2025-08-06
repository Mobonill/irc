#include "Connexion.hpp"
#include "Channel.hpp"

Connexion::Connexion() : _socket(0), _status(NOT_AUTHENTCATD), _ip(""),
_nickname(""), _username(""), _servername(""), _hostname(""), _realname("") {}

Connexion::~Connexion() {}

const std::string &Connexion::getNickName() const { return _nickname; }

const std::string &Connexion::getUserName() const { return _username; }

const std::string &Connexion::getServerName() const { return _servername; }

const std::string &Connexion::getHostName() const { return _hostname; }

const std::string &Connexion::getRealName() const { return _realname; }

int Connexion::getSocket() const { return _socket; }

const std::string &Connexion::getIp() const { return _ip; }

const int &Connexion::getStatus() const { return _status; }

const std::string Connexion::getFullAddress() const
{
	std::string full_address = _nickname + "!" + _username +  "@" + _ip;
	std::cout << "Use of getFullAdd() = [" << full_address << "]\n";
	return full_address;
}

const std::map<std::string, Channel> &Connexion::getJoinedChannels() const { return _joined_channels; }

void Connexion::addJoinedChannel(const std::string &channel_name, const Channel &new_channel)
{ _joined_channels.insert(std::pair<std::string, Channel>(channel_name, new_channel)); }
