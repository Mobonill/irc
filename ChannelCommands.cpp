#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"
#include <cstdlib>
#include <sys/socket.h>
#include <sstream>

template <typename T>
std::string toString(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

bool Server::isAuthenticated(int fd) {
    std::map<int, Client>::iterator it = _clients.find(fd);
    if (it == _clients.end()) {
        return false;
    }   
    bool isReg = it->second.isRegistered();
    int status = it->second.getStatus();    
    return isReg && status >= REGISTRD;
}


void Server::sendError(int fd, const std::string &message) {
    send(fd, message.c_str(), message.length(), 0);
}

// ... in class Server "@Alice Bob @Carol"

std::string Channel::getClientNickList() const {
    std::string list;

    for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (!list.empty())
            list += " ";

        if (_operators.find(it->first) != _operators.end())
            list += "@";

        list += it->second->getNickName();
    }

    return list;
}

bool Channel::isValidChannelName(const std::string& name) {
    if (name.empty())
        return false;
    if (name[0] != '#' && name[0] != '&')
        return false;
    if (name.length() > 200)
        return false;
    for (size_t i = 0; i < name.length(); ++i) {
        char c = name[i];
        if (c == ' ' || c == ',' || c == 7)
            return false;
    }
    return true;
}


// ---------------- JOIN ------------------
void Server::checkJoin(const std::vector<std::string> &command, int fd) {
    if (!isAuthenticated(fd)) {
        sendError(fd, ":localhost 451 JOIN :You have not registered\r\n");
        return;
    }
    if (command.size() < 2) {
        sendError(fd, ":localhost 461 JOIN :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
        return;
    }
    if (!_clients[fd].isRegistered()) {
        sendError(fd, ":localhost 451 JOIN :You have not registered\r\n");
        return;
    }
    if (command[1] == "0") {
        handleJoinZero(fd);
        return;
    }
    handleJoinChannels(fd, command);
}

void Server::handleJoinZero(int fd) {
    std::map<int, Client>::iterator clIt = _clients.find(fd);
    if (clIt == _clients.end())
        return;
    Client &client = clIt->second;

    std::vector<std::string> toLeaveListChannels;
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->second.hasClient(fd))
            toLeaveListChannels.push_back(it->first);
    }

    for (size_t i = 0; i < toLeaveListChannels.size(); ++i) {
        std::map<std::string, Channel>::iterator chIt = _channels.find(toLeaveListChannels[i]);
        if (chIt == _channels.end())
            continue;
        Channel &channel = chIt->second;

        std::string partMsg = ":" + client.getNickName() + "!~user@localhost PART " + channel.getName() + "\r\n";
        std::cout << "Removing client " << client.getNickName()
                  << " from channel " << channel.getName() << "\n";
        channel.broadcast(partMsg);
        channel.removeClient1(fd);
    }
    
    client.setStatus(REGISTRD);
    }

void Server::handleJoinChannels(int fd, const std::vector<std::string> &tokens) {

    std::vector<std::string> channels = splitString(tokens[1], ",");
    std::vector<std::string> keys;
    if (tokens.size() >= 3)
        keys = splitString(tokens[2], ",");

    Client &client = _clients[fd];

    //  (ERR_TOOMANYCHANNELS)
    if (channels.size() > 100) {
        sendError(fd, ":localhost 405 JOIN :You have joined too many channels\r\n");
        return;
    }

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string key = (i < keys.size()) ? keys[i] : "";
        handleSingleChannelJoin(fd, client, channels[i], key);
    }
}


void Server::handleSingleChannelJoin(int fd, Client &client, const std::string &channelName, const std::string &providedKey) {
    if (!Channel::isValidChannelName(channelName)) {
        sendError(fd, ":localhost 476 " + channelName + " :Invalid channel name\r\n"); // ERR_BADCHANMASK
        return;
    }

    Channel &channel = getOrCreateChannel(channelName, fd);
    if (channel.hasClient(fd)) {
       // std::cout << "[DEBUG][handleSingleChannelJoin] Client already in channel" << std::endl;
        return;
    }

    std::string channelPassword = channel.getPassword();

    if (!channelPassword.empty() && channelPassword != providedKey) {
        sendError(fd, ":localhost 475 " + channelName + " :Cannot join channel (+k) - wrong key\r\n"); // ERR_BADCHANNELKEY
        return;
    }
    
    if (channelPassword.empty() && !providedKey.empty()) {
        channel.setPassword(providedKey);
    }
    if (!channelPassword.empty() && providedKey.empty()) {
        sendError(fd, ":localhost 475 " + channelName + " :Cannot join channel (+k) - wrong key\r\n"); // ERR_BADCHANNELKEY
        return;
    }

    if (channel.getInviteOnly() && !channel.isOperator(fd) && !channel.isInvited(fd)) {
        sendError(fd, ":localhost 473 " + channelName + " :Cannot join channel (+i)\r\n"); // ERR_INVITEONLYCHAN
        return;
    }

    if (channel.isFull()) {
        sendError(fd, ":localhost 471 " + channelName + " :Channel is full\r\n"); // ERR_CHANNELISFULL
        return;
    }
    
    if (!channel.addClient1(&client)) {
        sendError(fd, ":localhost 471 " + channelName + " :Channel is full\r\n"); // ERR_CHANNELISFULL
        return;
    }
    
    if (channel.getOperatorCount() == 0) {
        channel.promoteOperator(fd);
    }

    client.addJoinedChannel(channelName, channel);
    
    channel.removeInvite(fd);

    client.setStatus(IN_CHANNEL);
 
    sendJoinMessages(fd, client, channel);
}

Channel& Server::getOrCreateChannel(const std::string &channelName, int fd) {
    std::map<std::string, Channel>::iterator chIt = _channels.find(channelName);
    if (chIt == _channels.end()) {
        chIt = _channels.insert(std::make_pair(channelName, Channel(channelName))).first;
        chIt->second.promoteOperator(fd);
    }
    return chIt->second;
}

void Server::sendJoinMessages(int fd, Client &client, Channel &channel) {
    std::string channelName = channel.getName();
    std::cout << "Client " << _clients[fd].getNickName() << " joins channel " << channelName << std::endl;

    std::string joinMsg = ":" + client.getNickName() + "!~user@localhost JOIN :" + channelName + "\r\n";
    channel.broadcast(joinMsg, fd);
    if (!channel.getTopic().empty()) {
        std::string topicMsg = ":localhost 332 " + client.getNickName() + " " + channelName + " :" + channel.getTopic() + "\r\n"; // RPL_TOPIC
        send(fd, topicMsg.c_str(), topicMsg.length(), 0);

        if (!channel.getTopicSetter().empty() && channel.getTopicSetTime() != 0) {
            std::ostringstream oss;
            oss << channel.getTopicSetTime();
            std::string timeStr = oss.str();

            std::string topicWhoTimeMsg = ":localhost 333 " + client.getNickName() + " " + channelName + " " +
                                         channel.getTopicSetter() + " " + timeStr + "\r\n"; // RPL_TOPICWHOTIME
            send(fd, topicWhoTimeMsg.c_str(), topicWhoTimeMsg.length(), 0);
        }
    } else {
        std::string noTopicMsg = ":localhost 331 " + client.getNickName() + " " + channelName + " :No topic is set\r\n"; // RPL_NOTOPIC
        send(fd, noTopicMsg.c_str(), noTopicMsg.length(), 0);
    }

    std::string namesList = ":localhost 353 " + client.getNickName() + " = " + channelName + " :" + channel.getClientNickList() + "\r\n"; // RPL_NAMREPLY
    std::string endOfNames = ":localhost 366 " + client.getNickName() + " " + channelName + " :End of /NAMES list\r\n"; // RPL_ENDOFNAMES
    send(fd, namesList.c_str(), namesList.length(), 0);
    send(fd, endOfNames.c_str(), endOfNames.length(), 0);
}


// ---------------- KICK ------------------
//KICK #channel targetNick
void Server::checkKick(const std::vector<std::string> &command, int fd) {

    if (!isAuthenticated(fd)) {
        sendError(fd, ":localhost 451 KICK :You have not registered\r\n");
        return;
    }
    
    if (command.size() < 3) {
        sendError(fd, ":localhost 461 KICK :Not enough parameters\r\n");  // ERR_NEEDMOREPARAMS
        return;
    }

    std::string channelName = command[1];
    std::string targetNick = command[2];

    if (!Channel::isValidChannelName(channelName)) {
        sendError(fd, ":localhost 476 " + channelName + " :Invalid channel name\r\n");  // ERR_BADCHANMASK
        return;
    }

    Channel* channel = getChannelOrSendError(fd, channelName);
    if (!channel)
        return;

    if (!channel->hasClient(fd)) {
        sendError(fd, ":localhost 442 " + channelName + " :You're not on that channel\r\n");  // ERR_NOTONCHANNEL
        return;
    }
    if (!channel->isOperator(fd)) {
        sendError(fd, ":localhost 482 " + channelName + " :You're not channel operator\r\n");  // ERR_CHANOPRIVSNEEDED
        return;
    }

    int targetFd = findClientFdByNick(targetNick);

    if (targetFd == -1 || !channel->hasClient(targetFd)) {
        sendError(fd, ":localhost 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n");  // ERR_USERNOTINCHANNEL
        return;
    }

    Client* client = getClientOrSendError(fd);
    if (!client)
        return;

    std::string reason = "kicked";
    if (command.size() > 3) {
        reason.clear();
        for (size_t i = 3; i < command.size(); ++i) {
            if (i > 3) reason += " ";
            reason += command[i];
        }
        if (!reason.empty() && reason[0] == ':')
            reason = reason.substr(1);
    }

    executeKick(*channel, *client, targetFd, targetNick, channelName, reason);
}

Channel* Server::getChannelOrSendError(int fd, const std::string& channelName) {
    std::map<std::string, Channel>::iterator chIt = _channels.find(channelName);
    if (chIt == _channels.end()) {
        sendError(fd, ":localhost 403 " + channelName + " :No such channel\r\n");  // ERR_NOSUCHCHANNEL
        return NULL;
    }
    return &chIt->second;
}

int Server::findClientFdByNick(const std::string& nick) const {
    for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second.getNickName() == nick)
            return it->first;
    }
    return -1;
}

Client* Server::getClientOrSendError(int fd) {
    std::map<int, Client>::iterator it = _clients.find(fd);
    if (it == _clients.end()) {
        sendError(fd, ":localhost 451 :Client not found\r\n");  // ERR_NOTREGISTERED (используем как заглушку)
        return NULL;
    }
    return &it->second;
}

void Server::executeKick(Channel& channel, Client& kicker, int targetFd, const std::string& targetNick, const std::string& channelName, const std::string& reason) {
    std::string kickMsg = ":" + kicker.getNickName() + "!~user@localhost KICK " +
                          channelName + " " + targetNick + " :" + reason + "\r\n";

    std::cout << "User " << kicker.getNickName() << " kicked " << targetNick 
              << " from channel " << channelName << std::endl;

    channel.broadcast(kickMsg);
    bool wasOperator = channel.isOperator(targetFd);

    channel.removeClient1(targetFd);
    channel.removeClient(targetFd);

    int remainingClients = channel.getClientCount();

    if (remainingClients == 0) {
        _channels.erase(channelName);
    }
    else if (wasOperator && channel.getOperatorCount() == 0 && remainingClients > 0) {
        const std::map<int, Client*>& clients = channel.getClients();
        if (!clients.empty()) {
            int firstClientFd = clients.begin()->first;
            channel.promoteOperator(firstClientFd);
        }
    }
    
}

// ---------------- TOPIC ------------------
//TOPIC #chat :Welcome to the general chat

void Server::checkTopic(const std::vector<std::string> &command, int fd) {

    if (!isAuthenticated(fd)) {
        sendError(fd, ":localhost 451 TOPIC :You have not registered\r\n");
        return;
    }

    if (command.size() < 2) {
        sendError(fd, ":localhost 461 TOPIC :Not enough parameters\r\n");
        return;
    }

    std::string channelName = command[1];
    std::map<std::string, Channel>::iterator chIt = _channels.find(channelName);
    if (chIt == _channels.end()) {
        sendError(fd, ":localhost 403 " + channelName + " :No such channel\r\n");
        return;
    }

    Channel &channel = chIt->second;
    Client &client = _clients[fd];

    if (!channel.hasClient(fd)) {
        sendError(fd, ":localhost 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (command.size() == 2) {
        std::string topic = channel.getTopic();
        if (!topic.empty()) {
            std::string msg = ":localhost 332 " + client.getNickName() + " " + channelName + " :" + topic + "\r\n";
            send(fd, msg.c_str(), msg.length(), 0);

            std::ostringstream oss;
            oss << channel.getTopicSetTime();
            std::string timeStr = oss.str();

            msg = ":localhost 333 " + client.getNickName() + " " + channelName + " " +
                  channel.getTopicSetter() + " " + timeStr + "\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
        } else {
            std::string msg = ":localhost 331 " + client.getNickName() + " " + channelName + " :No topic is set\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
        }
        return;
    }

    if (channel.getTopicRestricted() && !channel.isOperator(fd)) {
        sendError(fd, ":localhost 482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    std::string newTopic;
    for (size_t i = 2; i < command.size(); ++i) {
        if (i > 2) newTopic += " ";
        newTopic += command[i];
    }

    if (!newTopic.empty() && newTopic[0] == ':')
        newTopic = newTopic.substr(1);

    channel.setTopic(newTopic, client.getNickName());
    std::cout << "User " << client.getNickName() << " changed topic on channel " << channelName
          << " to: " << newTopic << std::endl;

    std::string notify = ":" + client.getNickName() + "!~user@localhost TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel.broadcast(notify);
}

// ---------------- INVITE ------------------
//INVITE Alice #secret
void Server::checkInvite(const std::vector<std::string> &command, int fd) {
    if (!isAuthenticated(fd)) {
        sendError(fd, ":localhost 451 INVITE :You have not registered\r\n");
        return;
    }

    if (command.size() < 3) {
        sendError(fd, ":localhost 461 INVITE :Not enough parameters\r\n");
        return;
    }

    std::string nick = command[1];
    std::string channelName = command[2];

    std::map<std::string, Channel>::iterator chIt = _channels.find(channelName);
    if (chIt == _channels.end()) {
        sendError(fd, ":localhost 403 " + channelName + " :No such channel\r\n");
        return;
    }

    Channel &channel = chIt->second;

    if (!channel.hasClient(fd)) {
        sendError(fd, ":localhost 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (!channel.isOperator(fd)) {
        sendError(fd, ":localhost 482 " + channelName + " :You're not channel operator\r\n");
        return;
    }

    int targetFd = -1;
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second.getNickName() == nick) {
            targetFd = it->first;
            break;
        }
    }

    if (targetFd == -1) {
        sendError(fd, ":localhost 401 " + nick + " :No such nick/channel\r\n");
        return;
    }

    if (channel.hasClient(targetFd)) {
        sendError(fd, ":localhost 443 " + nick + " " + channelName + " :is already on channel\r\n");
        return;
    }

    channel.inviteClient(targetFd);

    Client &client = _clients[fd];
    std::string inviteMsg = ":" + client.getNickName() + "!~user@localhost INVITE " + nick + " :" + channelName + "\r\n";
    send(targetFd, inviteMsg.c_str(), inviteMsg.length(), 0);

    std::string rplInviting = ":localhost 341 " + client.getNickName() + " " + nick + " " + channelName + "\r\n";
    send(fd, rplInviting.c_str(), rplInviting.length(), 0);
    std::cout << "User " << client.getNickName() << " invited " << nick << " to channel " << channelName << std::endl;
}


// ---------------- MODE ------------------

/*1. MODE #channel +i — Enables invite-only mode. Users can only join if they are invited.
2. MODE #channel -i — Disables invite-only mode, allowing anyone to join.
3. MODE #channel +t — Only channel operators can change the topic.
4. MODE #channel -t — Allows any user in the channel to change the topic.
5. MODE #channel +l 10 — Sets a user limit of 10. Only 10 users can be in the channel at once.
6. MODE #channel -l — Removes the user limit.
7. MODE #channel +k secretpass — Sets a password (secretpass) required to join the channel.
8. MODE #channel -k — Removes the channel password.
9. MODE #channel +o Alice — Grants operator privileges to user Alice.
10. MODE #channel -o Alice — Removes operator privileges from user Alice*/

int Server::getClientFdByNick(const std::string& nick) {
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second.getNickName() == nick)
            return it->first;
    }
    return -1;
}

void Server::sendCurrentModes(int fd, Client &client, Channel &channel, const std::string &target) {
    std::string modeStr = "+";
    std::string params;

    if (channel.getInviteOnly()) modeStr += "i";
    if (channel.getTopicRestricted()) modeStr += "t";
    if (!channel.getPassword().empty() && channel.isOperator(fd)) {
        modeStr += "k";
        params += " " + channel.getPassword();
    } else if (!channel.getPassword().empty()) {
        modeStr += "k";
    }
    if (channel.getUserLimit() > 0) {
        modeStr += "l";
        std::ostringstream oss;
        oss << channel.getUserLimit();
        params += " " + oss.str();
    }
    std::string reply = ":localhost 324 " + client.getNickName() + " " + target + " " + modeStr + params + "\r\n";
    send(fd, reply.c_str(), reply.length(), 0);

    std::ostringstream oss;
    oss << channel.getCreationTime();
    std::string creationReply = ":localhost 329 " + client.getNickName() + " " + target + " " + oss.str() + "\r\n";
    send(fd, creationReply.c_str(), creationReply.length(), 0);
}

bool Server::processModeChange(int fd, const std::vector<std::string> &tokens, Channel &channel, Client &client, const std::string &target) {
    std::string modeStr = tokens[2];
    bool adding = true;
    size_t paramIndex = 3;
    std::string unknownModes;

    for (size_t i = 0; i < modeStr.size(); ++i) {
        char mode = modeStr[i];

        if (mode == '+') { adding = true; continue; }
        if (mode == '-') { adding = false; continue; }

        switch (mode) {
            case 'i':
                channel.setInviteOnly(adding);
                break;

            case 't':
                channel.setTopicRestricted(adding);
                break;

            case 'k':
                if (adding) {
                    if (paramIndex >= tokens.size()) {
                        sendError(fd, ":localhost 461 MODE :Missing parameter for +k\r\n");
                        return false;
                    }
                    channel.setPassword(tokens[paramIndex++]);
                } else {
                    channel.setPassword("");
                }
                break;

            case 'o': {
                if (paramIndex >= tokens.size()) {
                    sendError(fd, ":localhost 461 MODE :Missing parameter for +o/-o\r\n");
                    return false;
                }
                std::string nick = tokens[paramIndex++];
                int targetFd = getClientFdByNick(nick);
                if (targetFd == -1 || !channel.hasClient(targetFd)) {
                    sendError(fd, ":localhost 441 " + nick + " " + target + " :They aren't on that channel\r\n");
                    return false;
                }
                if (adding)
                    channel.promoteOperator(targetFd);
                else
                    channel.demoteOperator(targetFd);
                break;
            }

            case 'l':
                if (adding) {
                    if (paramIndex >= tokens.size()) {
                        sendError(fd, ":localhost 461 MODE :Missing parameter for +l\r\n");
                        return false;
                    }
                    int limit = atoi(tokens[paramIndex++].c_str());
                    if (limit > 0)
                        channel.setUserLimit(limit);
                    else
                        channel.setUserLimit(0);
                } else {
                    channel.setUserLimit(0);
                }
                break;

            default:
                unknownModes += mode;
                break;
        }
    }

    if (!unknownModes.empty()) {
        sendError(fd, ":localhost 501 " + unknownModes + " :Unknown MODE flag(s)\r\n");
    }
    std::string modeMsg = ":" + client.getNickName() + "!~user@localhost MODE " + target + " " + modeStr;
    for (size_t i = 3; i < tokens.size(); ++i) {
        modeMsg += " " + tokens[i];
    }
    modeMsg += "\r\n";
    channel.broadcast(modeMsg);
    std::cout << "User " << client.getNickName() << " set mode " << modeStr 
              << " on channel " << target << std::endl;
    return true;
}

bool Server::isValidChannelName1(const std::string& channelName) {
    if (channelName.empty()) return false;
    char prefix = channelName[0];
    return (prefix == '#' || prefix == '&');
}

void Server::checkMode(const std::vector<std::string> &command, int fd) {
    if (!isAuthenticated(fd)) {
        sendError(fd, ":localhost 451 MODE :You have not registered\r\n");
        return;
    }

    if (command.size() < 2) {
        sendError(fd, ":localhost 461 MODE :Not enough parameters\r\n");
        return;
    }

    std::string target = command[1];
    if (target.empty()) {
        sendError(fd, ":localhost 461 MODE :Not enough parameters\r\n");
        return;
    }

    if (target[0] == '#' || target[0] == '&') {
        if (!isValidChannelName1(target)) {
            sendError(fd, ":localhost 476 " + target + " :Invalid channel name\r\n");
            return;
        }

        std::map<std::string, Channel>::iterator chIt = _channels.find(target);
        if (chIt == _channels.end()) {
            sendError(fd, ":localhost 403 " + target + " :No such channel\r\n");
            return;
        }

        Channel &channel = chIt->second;
        Client &client = _clients[fd];

        if (command.size() == 2) {
            sendCurrentModes(fd, client, channel, target);
            return;
        }

        if (!channel.isOperator(fd)) {
            sendError(fd, ":localhost 482 " + target + " :You're not channel operator\r\n");
            return;
        }

        processModeChange(fd, command, channel, client, target);
    }
    else {
        sendError(fd, ":localhost 502 :Cant change mode for other users\r\n");
    }
}


std::string Server::getServerCreationDate() {
    char buf[64];
    std::tm* tm_info = std::localtime(&_creationTime);
    if (tm_info)
        std::strftime(buf, sizeof(buf), "%a %b %d %Y", tm_info);
    else
        std::strcpy(buf, "Unknown");
    return std::string(buf);
}


void Server::sendWelcomeMessage(Client& client) {
    std::string nick = client.getNickName();
    int fd = client.getSocket();

    sendToClient(fd, ":localhost 001 " + nick + " :" +
        "Welcome to the Internet Relay Network " + nick + "!" +
        client.getUserName() + "@" + client.getHostName() + "\r\n");

    sendToClient(fd, ":localhost 002 " + nick + " :" +
        "Your host is localhost, running version 1.0\r\n");

    sendToClient(fd, ":localhost 003 " + nick + " :" +
        "This server was created " + getServerCreationDate() + "\r\n");

    sendToClient(fd, ":localhost 004 " + nick + " localhost 1.0 o o\r\n");
     std::cout << "Client registered successfully: " << client.getNickName() << std::endl;
}


//USER guest 0 * :Ronnie Reagan
//USER <username> <hostname> <servername> <parasite arg>:<realname>

void Server::checkUser(const std::vector<std::string>& command, int fd) {
    Client& client = _clients[fd];

    if (client.hasUserCommand()) {
        sendToClient(fd, ":localhost 462 * :You may not reregister\r\n");
        return;
    }

    if (command.size() < 5) {
        sendToClient(fd, ":localhost 461 * USER :Not enough parameters\r\n");
        return;
    }

    std::string username = command[1];
    std::string hostname = command[2];
    std::string servername = command[3];
    
    if (username.empty() || hostname.empty() || servername.empty()) {
        sendToClient(fd, ":localhost 461 * USER :Not enough parameters\r\n");
        return;
    }

    std::string realname;
    if (command[4][0] == ':') {
        realname = command[4].substr(1);
        for (size_t i = 5; i < command.size(); ++i)
            realname += " " + command[i];
    } else {
        sendToClient(fd, ":localhost 461 * USER :Not enough parameters\r\n");
        return;
    }

    if (realname.empty()) {
        sendToClient(fd, ":localhost 461 * USER :Not enough parameters\r\n");
        return;
    }

    if (hostname != "0" || servername != "*") {
        sendToClient(fd, ":localhost 461 * USER :Hostname and servername must be '0' and '*'\r\n");
        return;
    }

    client.setUserName(username);
    client.setHostName(hostname);
    client.setServerName(servername);
    client.setRealName(realname);
    client.setHasUserCommand(true);

    tryFinishRegistration(client);
}
