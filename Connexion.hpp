#pragma once

#include <string>
#include <map>
#include <vector>
#include <ctime>

class Channel;

enum stateMachine
{
	NOT_AUTHENTCATD,
	NOT_REGISTRD,
	REGISTRD,
	IN_CHANNEL
};

class Connexion
{
	protected:
		int _socket;
		int _status;
		std::string	_ip;

		std::string	_nickname;
		std::string _username;
		std::string _servername;
		std::string _hostname;
		std::string _realname;
		
		std::map<std::string, Channel> _joined_channels;
		std::string _buffer;

		Connexion();
	public:
		virtual ~Connexion();

		const std::string &getNickName() const;
		const std::string &getUserName() const;
		const std::string &getServerName() const;
		const std::string &getHostName() const;
		const std::string &getRealName() const;

		int getSocket() const;
		const std::string &getIp() const;
		const int &getStatus() const;
		const std::map<std::string, Channel> &getJoinedChannels() const;
		void addJoinedChannel(const std::string &channel_name, const Channel &new_channel);
};