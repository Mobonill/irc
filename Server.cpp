/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:17 by morgane           #+#    #+#             */
/*   Updated: 2025/06/11 23:17:30 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.cpp"

Server* g_signal = NULL;

Server::Server(int port, const std::string &password): _port(port), _password(password), _socketFd(-1) {
    _clientsNumber = 0;
    _signal = false;
    
}

Server::~Server() {

}



bool Server::getSignal() {

    return (this->_signal);
}


void Server::handleSignal(int signal)
{
	(void)signal;
	std::cout << std::endl << "handle signal function worked" << std::endl;
	if (g_signal)
        g_signal->stopSignal();
    else
        std::cerr << "g_signal not initialized!" << std::endl;
}

void Server::stopSignal() {
    _signal = true;
}


void Server::closeFds(){

    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); )
    {
        std::cout << "Client <" << it->first << "> disconnected" << std::endl;
        close(it->first);
        _clients.erase(it++);
    }

    if (_socketFd != -1)
        close(_socketFd);
}

void Server::initServer(void) {
}

void Server::createServer(void)
{

    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd == -1)
        throw std::runtime_error("Failed to create socket");
    else
    {
        std::cout << "Socket created" << std::endl;
        int opt = 1;
        setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    sockaddr_in serverAddress;

    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_port);

    if (bind(_socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        throw ::std::runtime_error("Socket bind failed");
    }
    else
    {
        std::cout << "Socket binded" << std::endl;

        if (listen(_socketFd, SOMAXCONN ) < 0) {
            std::cerr << strerror(errno) << std::endl;
            throw std::runtime_error("Listen failed");
        }
        else
            std::cout << "Server is listening" << std::endl;
    }
    if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set socket non-blocking");

    std::cout << "Server created and listening on port " << _port << std::endl;

}

void Server::socketChecker() {
    
    fd_set allFds;
    
    
    FD_ZERO(&allFds);
    FD_SET(_socketFd, &allFds);
    _max_fd = _socketFd;
    
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        FD_SET(it->first, &allFds);
        if (it->first > _socketFd)
            _max_fd = it->first;
    }

    int connexion = select(_max_fd + 1, &allFds, NULL, NULL, NULL);
    if (connexion < 0)
        throw std::runtime_error("Select failed");

    if (FD_ISSET(_socketFd, &allFds))
        handleNewClient();
    
    std::vector<int> clients_to_remove;

    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (FD_ISSET(it->first, &allFds)) {
               if (!handleClientMessage(it->first)) {
                   clients_to_remove.push_back(it->first);
               }
           }
     }
     for (size_t i = 0; i < clients_to_remove.size(); ++i)
         clearClient(clients_to_remove[i]);
            
}

void Server::handleNewClient() {
    
        sockaddr_in client_addr;
        socklen_t client_addrLen = sizeof(client_addr);
        int clientSocket;
        
        clientSocket = accept(_socketFd, (sockaddr*)&client_addr, &client_addrLen);
        if (clientSocket < 0) {
            throw std::runtime_error("Accept error");
        } 
        else {
            std::cout << "New client connected: Client Socker :" << clientSocket << std::endl;
            fcntl(clientSocket, F_SETFL, O_NONBLOCK);
            _clients.insert(std::make_pair(clientSocket, Client(clientSocket)));
            if (clientSocket > _clientsNumber)
                _clientsNumber = clientSocket;
        }
}

bool Server::handleClientMessage(int client_fd) {

    char buff[4000];
    memset(buff, 0, sizeof(buff));
    
    ssize_t bytes = recv(client_fd, buff, sizeof(buff) - 1, 0);
    if (bytes <= 0) {
        std::cout << "Client " << client_fd << " disconnected" << std::endl;
        return (false);
    }
    else {
        buff[bytes] = '\0';
        std::string msg(buff);
        parseMessage(client_fd, msg);
        return (true);
    }
    
}

void Server::clearClient(int client_fd) {
    
    std::map<int, Client>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        close(client_fd);
        _clients.erase(it);
        std::cout << "Client <" << client_fd << "> erased" << std::endl;
    }
}


void Server::parseMessage(int client_fd, const std::string &msg) {
    
    _clientBuffers[client_fd] += msg;

    std::string &buffer = _clientBuffers[client_fd];
    size_t pos;

    while ((pos = buffer.find("\n")) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos);
        if (!line.empty() && line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);
        buffer.erase(0, pos + 1);
        parseAndExecute(client_fd, line);
    }
}

void Server::parseAndExecute(int client_fd, std::string line) {

    if (line.empty())
        return;
    
    std::vector<std::string> cmds;
    cmds.push_back(line);
        
    handleCommands(client_fd, cmds);
}

void Server::handleCommands(int fd, const std::vector<std::string> &vectorCmd) {
	
    std::string cmd;
	std::vector<std::string> vectorSpliter;

	for (size_t i = 0; i < vectorCmd.size(); i++) {
		cmd = vectorCmd[i];
		vectorSpliter = splitString(cmd, " ");
		if (vectorSpliter.empty())
			continue;
		if (vectorSpliter[0] == "PASS") {
			// checkPass(fd, cmd);
		}
		else if (vectorSpliter[0] == "NICK") {
			// checkNick(fd, cmd);
		}
		else if (vectorSpliter[0] == "USER") {
			// checkUser(fd, cmd);
		}
		else if (vectorSpliter[0] == "JOIN") {
			// checkJoin(fd, cmd);
		}
        else if (vectorSpliter[0] == "TOPIC") {
			// checkTopic(fd, cmd);
		}
        else if (vectorSpliter[0] == "KICK") {
			// checkKick(fd, cmd);
		}
        else if (vectorSpliter[0] == "MODE") {
			// checkMode(fd, cmd);
		}
        else if (vectorSpliter[0] == "INFO") {
			// checkInfo(fd, cmd);
		}
        else if (vectorSpliter[0] == "INVITE") {
			// checkInvite(fd, cmd);
		}
        else if (vectorSpliter[0] == "PRIVMSG") {
			// checkPrivmsg(fd, cmd);
		}
        else if (vectorSpliter[0] == "BOT\r") {
			// checkBot(fd, cmd);
		}
		else {
			std::cout << "-> Unknown command " << fd << " : " << vectorSpliter[0] << std::endl;
		}
	}
}


