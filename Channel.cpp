// Channel.cpp
#include "Channel.hpp"
#include "Client.hpp"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>


Channel::Channel(const std::string& name)
    : _name(name),
      _topic(""),
      _topicSetter(""),
      _topicSetTime(0),
      _creationTime(time(NULL)),
      _password(""),
      _userLimit(-1),
      _inviteOnly(false),
      _topicRestricted(false)
{}

bool Channel::addClient(Client* client) {
    if (isFull())
        return false;
    _clients[client->getClientSocket()] = client;
    return true;
}

void Channel::removeClient(int fd) {
	_clients.erase(fd);
	_operators.erase(fd);
}

bool Channel::hasClient(int fd) const {
	return _clients.find(fd) != _clients.end();
}

void Channel::promoteOperator(int fd) {
	_operators.insert(fd);
}

void Channel::demoteOperator(int fd) {
	_operators.erase(fd);
}

bool Channel::isOperator(int fd) const {
	return _operators.find(fd) != _operators.end();
}

void Channel::setInviteOnly(bool state) {
	_inviteOnly = state;
}

bool Channel::getInviteOnly() const {
	return _inviteOnly;
}

void Channel::setTopicRestricted(bool state) {
	_topicRestricted = state;
}

bool Channel::getTopicRestricted() const {
	return _topicRestricted;
}

void Channel::setUserLimit(int limit) {
	_userLimit = limit;
}

int Channel::getUserLimit() const {
	return _userLimit;
}

bool Channel::isFull() const {
	return _userLimit > 0 && (int)_clients.size() >= _userLimit;
}

void Channel::setPassword(const std::string& pass) {
	_password = pass;
}

bool Channel::checkPassword(const std::string& pass) const {
	return _password == pass;
}

void Channel::clearPassword() {
	_password.clear();
}

void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

const std::string& Channel::getTopic() const {
	return _topic;
}

const std::string& Channel::getName() const {
	return _name;
}

void Channel::broadcast(const std::string& msg, int excludeFd) {
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->first != excludeFd) {
			send(it->first, msg.c_str(), msg.length(), 0);
		}
	}
}

bool Channel::isInvited(int fd) const {
    return _invited.find(fd) != _invited.end();
}

void Channel::inviteClient(int fd) {
    _invited.insert(fd);
}

void Channel::removeInvite(int fd) {
    _invited.erase(fd);
}

std::string Channel::getPassword() const {
    return _password;
}

  void Channel::setTopic(const std::string& topic, const std::string& setter) {
        _topic = topic;
        _topicSetter = setter;
        _topicSetTime = time(0);
    }

    const std::string& Channel::getTopicSetter() const { return _topicSetter; }
    time_t Channel::getTopicSetTime() const { return _topicSetTime; }

	bool Channel::isBanned(int fd) const {
    return _bannedClients.find(fd) != _bannedClients.end();
}

time_t Channel::getCreationTime() const {
    return _creationTime;
}