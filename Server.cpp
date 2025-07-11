/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:17 by morgane           #+#    #+#             */
/*   Updated: 2025/07/11 17:38:43 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

Server* g_signal = NULL;

Server::Server(int port, const std::string &password): _port(port), _password(password), _socketFd(-1)
{
	_clientsNumber = 0;
	_signal = false;
}

Server::~Server() {}

void	Server::setServerName(const std::string &newName) { _serverName = newName; }

const std::map<std::string, Channel> &Server::getChannels() const { return _channels; }

bool Server::getSignal() { return (this->_signal); }

void Server::handleSignal(int signal)
{
	(void)signal;
	std::cout << std::endl << "handle signal function worked" << std::endl;
	if (g_signal)
		g_signal->stopSignal();
	else
		std::cerr << "g_signal not initialized!" << std::endl;
}

void Server::stopSignal() { _signal = true; }

void Server::closeFds()
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); )
	{
		std::cout << "Client <" << it->first << "> disconnected" << std::endl;
		close(it->first);
		_clients.erase(it++);
	}

	if (_socketFd != -1)
		close(_socketFd);
}

void Server::initServer(void) {}

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
	// socklen_t len = sizeof(serverAddress);
	// getsockname(_socketFd, (struct sockaddr *)&serverAddress, &len);
	// cinet_ntoa(serverAddress.sin_addr);
	_serverName = "irc.localhost";
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set socket non-blocking");

	std::cout << "Server created and listening on port " << _port << std::endl;
}

void Server::socketChecker()
{
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

void Server::handleNewClient()
{
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
		_clients[clientSocket].setIp(inet_ntoa(client_addr.sin_addr));
	}
}

bool Server::handleClientMessage(int client_fd)
{
	char buff[4000];
	memset(buff, 0, sizeof(buff));
	
	ssize_t bytes = recv(client_fd, buff, sizeof(buff) - 1, 0);
	if (bytes <= 0)
	{
		std::cout << "Client " << client_fd << " disconnected" << std::endl;
		return (false);
	}
	else
	{
		buff[bytes] = '\0';
		std::string msg(buff);
		parseMessage(client_fd, msg);
		return (true);
	}
}

void Server::clearClient(int client_fd)
{
	std::map<int, Client>::iterator it = _clients.find(client_fd);
	if (it != _clients.end()) {
		close(client_fd);
		_clients.erase(it);
		std::cout << "Client <" << client_fd << "> erased" << std::endl;
	}
}

void Server::parseMessage(int client_fd, const std::string &msg)
{
	_clientBuffers[client_fd] += msg;

	std::string &buffer = _clientBuffers[client_fd];
	size_t pos;

	while ((pos = buffer.find("\n")) != std::string::npos)
	{
		std::string line = buffer.substr(0, pos);
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
		buffer.erase(0, pos + 1);
		parseCommands(client_fd, line);
	}
}

void Server::parseCommands(int fd, const std::string &line)
{
	if (line.empty())
		return ;
	std::vector<std::string> split_cmdline = splitString(line, " ");
	if (split_cmdline.empty())
		return ;
	std::string cmdsArray[] = { "STATUS","PASS","NICK","USER","JOIN","TOPIC","KICK","MODE","INFO","INVITE","PRIVMSG","BOT"};
	std::set<std::string> cmdsCatalog(cmdsArray, cmdsArray + sizeof(cmdsArray) / sizeof(std::string));
	if (cmdsCatalog.find(split_cmdline[0]) != cmdsCatalog.end())
			handleCommands(fd, split_cmdline);
	else
		sendServerMessage(fd, "421", split_cmdline[0] + " :Unknown command");
}

void Server::handleCommands(int fd, const std::vector<std::string> &command)
{
	int status = _clients[fd].getStatus();
	bool authenticated = _clients[fd].getAuthenticated();

	std::cout << "DEBUG: Client " << fd << " - Status: " << status \
	<< ", Authenticated: " << authenticated \
	<< ", Command: '" << command[0] << "'" << std::endl;
	if (command[0] == "STATUS")
		checkStatus(fd);
	else if (status >= NOT_AUTHENTCATD && command[0] == "PASS") //store the password
	{
		std::cout << "DEBUG: Calling checkPass" << std::endl;
		checkPass(command, fd);
	}
	else if (status >= NOT_REGISTRD && command[0] == "NICK") //set the nickname
		checkNick(command, fd);
	else if (status >= NOT_REGISTRD && command[0] == "USER") //register user info
		checkUser(command, fd);
	// else if (status >= REGISTRD && command[0] == "JOIN") //demand access to a room(or create a new room and join it)
	// {
	// 	// checkJoin(fd, cmd);
	// }
	else if (status >= REGISTRD && command[0] == "INFO") // Lulu
		checkInfo(command, fd);
	// else if (status >= IN_CHANNEL && command[0] == "TOPIC") {
	// 	// checkTopic(fd, cmd);
	// }
	// else if (status >= IN_CHANNEL && command[0] == "KICK") {
	// 	// checkKick(fd, cmd);
	// }
	// else if (status >= IN_CHANNEL && command[0] == "MODE") {
	// 	// checkMode(fd, cmd);
	// }
	// else if (status >= IN_CHANNEL && command[0] == "INVITE") {
	// 	// checkInvite(fd, cmd);
	// }
	else if (status >= REGISTRD && command[0] == "PRIVMSG") // Lulu //target a specific channel(room) and send a message to all other clients
	{
		checkPrivmsg(command, fd);
	}
	else if (status >= REGISTRD && command[0] == "BOT") // Lulu
		checkBot(command, fd);
	else
		sendServerMessage(fd, "451", ":You have not registered");
}
