#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>


class Client;

class Channel {
private:
    std::string _name;       // Channel name (e.g., "#chat")
    std::string _topic;      // Current topic of the channel
    std::string _topicSetter;// Nickname of the user who set the topic
    time_t _topicSetTime;    // Time when the topic was last set
    time_t _creationTime;    // Time when the channel was created
    std::string _password;   // Channel password (if +k mode is set)
    int _userLimit;          // Max number of users allowed in the channel (+l mode)
    bool _inviteOnly;        // true if channel is invite-only (+i mode)
    bool _topicRestricted;   // true if only operators can change the topic (+t mode)

    std::map<int, Client*> _clients;
    std::set<int> _operators;
    std::set<int> _invited;
    std::set<int> _bannedClients;

public:
    Channel(const std::string& name);

    bool addClient(Client* client);
    void removeClient(int fd);
    bool hasClient(int fd) const;
    bool isOperator(int fd) const;
    void promoteOperator(int fd);
    void demoteOperator(int fd);
    bool isBanned(int fd) const;

    void setInviteOnly(bool state);
    bool getInviteOnly() const;

    void setTopicRestricted(bool state);
    bool getTopicRestricted() const;

    void setUserLimit(int limit);
    int getUserLimit() const;
    bool isFull() const;

    void setPassword(const std::string& pass);
    bool checkPassword(const std::string& pass) const;
    void clearPassword();

    void setTopic(const std::string& topic);
    const std::string& getTopic() const;
    void setTopic(const std::string& topic, const std::string& setter);
    const std::string& getTopicSetter() const;
    time_t getTopicSetTime() const;

    const std::string& getName() const;

    void broadcast(const std::string& msg, int excludeFd = -1);

    bool isInvited(int fd) const;
    void inviteClient(int fd);
    void removeInvite(int fd);
    std::string getClientNickList() const;
    std::string getPassword() const;
    static bool isValidChannelName(const std::string& name);
    time_t getCreationTime() const;

};
