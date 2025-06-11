/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/11 23:19:39 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <cerrno>
#include <sstream>
#include "Client.hpp"



class Channel {};

class Server {
    
    private:
    std::map<int, Client> _clients;
    std::map<std::string, Channel> _channels;
    std::map<int, std::string> _clientBuffers;
    int _port;
    std::string _password;
    int _socketFd;
    bool _signal;
    int _clientsNumber;
    int _max_fd;
    
    public:
    Server(int port, const std::string &password);
    ~Server();
    bool getSignal();
    void initServer();
    void createServer();
    void socketChecker();
    void handleNewClient();
    bool handleClientMessage(int client_fd);
    static void handleSignal(int signal);
    void closeFds();
    void stopSignal();
    void clearClient(int client_fd);
    void parseMessage(int client_fd, const std::string &msg);
    void parseAndExecute(int client_fd, std::string line);
    void handleCommands(int fd, const std::vector<std::string> &vectorCmd);
};


extern Server* g_signal;