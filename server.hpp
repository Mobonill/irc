/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:21 by morgane           #+#    #+#             */
/*   Updated: 2025/06/11 18:42:00 by morgane          ###   ########.fr       */
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



class Client {
    private:
    int _clientSocket;

    public:
    Client(int clientSock){_clientSocket = clientSock;};
    ~Client() {};
};

class Channel {};

class Server {
    
    private:
    std::map<int, Client> _clients;
    std::map<std::string, Channel> _channels;
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
    void handleClientMessage(int client_fd);
    static void handleSignal(int signal);
    void closeFds();
    void stopSignal();
    
    
};


extern Server* g_signal;