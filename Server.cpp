/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchauffo <lchauffo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:41:17 by morgane           #+#    #+#             */
/*   Updated: 2025/07/29 15:08:55 by lchauffo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

Server* g_signal = NULL;

Server::Server(int port, const std::string &password): _port(port), _password(password), _socket_fd(-1)
{
	_clients_number = 0;
	_signal = false;
	std::cout << "Server constructor: this = " << this << std::endl;
	#if BONUS
	 std::cout << "Creating Bot with server pointer: " << this << std::endl;
	 _bot = new Bot(this);
	 std::cout << "Bot created successfully" << std::endl;
	#endif
}

Server::~Server()
{
	#if BONUS
	 delete _bot;
	#endif
}

void	Server::setServerName(const std::string &new_name) { _server_name = new_name; }

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

	if (_socket_fd != -1)
		close(_socket_fd);
}

void Server::initServer(void) {}

void Server::createServer(void)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
		throw std::runtime_error("Failed to create socket");
	else
	{
		std::cout << "Socket created" << std::endl;
		int opt = 1;
		setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}

	sockaddr_in serverAddress;

	std::memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(_port);

	if (bind(_socket_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << strerror(errno) << std::endl;
		throw ::std::runtime_error("Socket bind failed");
	}
	else
	{
		std::cout << "Socket binded" << std::endl;

		if (listen(_socket_fd, SOMAXCONN ) < 0) {
			std::cerr << strerror(errno) << std::endl;
			throw std::runtime_error("Listen failed");
		}
		else
			std::cout << "Server is listening" << std::endl;
	}
	_server_name = "irc.localhost";
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set socket non-blocking");

	std::cout << "Server created and listening on port " << _port << std::endl;
	 #if BONUS
	// Activate bot after server is ready
	activateBot();
	#endif
}

void Server::socketChecker()
{
	fd_set all_fds;

	FD_ZERO(&all_fds);
	FD_SET(_socket_fd, &all_fds);
	_max_fd = _socket_fd;

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		FD_SET(it->first, &all_fds);
		if (it->first > _socket_fd)
			_max_fd = it->first;
	}

	int connexion = select(_max_fd + 1, &all_fds, NULL, NULL, NULL);
	if (connexion < 0)
		throw std::runtime_error("Select failed");

	if (FD_ISSET(_socket_fd, &all_fds))
		handleNewClient();
	
	std::vector<int> clients_to_remove;

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (FD_ISSET(it->first, &all_fds)) {
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
	socklen_t client_addr_len = sizeof(client_addr);
	int client_socket;
	
	client_socket = accept(_socket_fd, (sockaddr*)&client_addr, &client_addr_len);
	if (client_socket < 0) {
		throw std::runtime_error("Accept error");
	} 
	else {
		std::cout << "New client connected: Client Socker :" << client_socket << std::endl;
		fcntl(client_socket, F_SETFL, O_NONBLOCK);
		_clients.insert(std::make_pair(client_socket, Client(client_socket)));
		if (client_socket > _clients_number)
			_clients_number = client_socket;
		_clients[client_socket].setIp(inet_ntoa(client_addr.sin_addr));
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
	_client_buffers[client_fd] += msg;

	std::string &buffer = _client_buffers[client_fd];
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
	std::vector<std::string> split_cmdline;
	std::string msg = "";
	std::string cmd_line = line;
	std::string endspaces;

	if (line.empty())
		return ;
	size_t detect_msg = line.find(":");
	if (detect_msg != std::string::npos)
	{
		msg = line.substr(detect_msg);
		cmd_line = line.substr(0, detect_msg);
	}
	int last_non_space = static_cast<int>(cmd_line.size()) - 1;
	for (; last_non_space >= 0 ; --last_non_space)
		if (!std::isspace(static_cast<unsigned char>(cmd_line[last_non_space])))
			break ;
	if (static_cast<size_t>(last_non_space) < cmd_line.size() - 1)
	{
		endspaces = cmd_line.substr(last_non_space + 1);
		cmd_line.erase(last_non_space + 1);
	}
	split_cmdline = splitString(cmd_line, " ");
	if (split_cmdline.empty() || split_cmdline[0].empty())
		return ;
	if (msg.empty() && split_cmdline.size() > 1 && !endspaces.empty())
		split_cmdline.back() += endspaces;
	if (!msg.empty())
		split_cmdline.push_back(msg);
	std::string cmds_array[] = { "VERSION","STATUS","PASS","NICK","USER","JOIN","TOPIC","KICK","MODE","INFO","INVITE","PRIVMSG" };
	std::set<std::string> cmds_catalog(cmds_array, cmds_array + sizeof(cmds_array) / sizeof(std::string));
	#if BONUS
	//  cmds_catalog.insert("BOT");
	 std::string bonus_cmds[] = { "BOT","SERVER","COLOR" };//for multiple bonus command if needed
	 cmds_catalog.insert(bonus_cmds, bonus_cmds + sizeof(bonus_cmds) / sizeof(std::string));
	#endif
	std::cout << "split_cmdline.size() = [" << split_cmdline.size() << "]\n";//DEBUG lines
	for (std::vector<std::string>::iterator vit = split_cmdline.begin(); vit != split_cmdline.end(); ++vit)//DEBUG lines
		std::cout << "-- *vit = [" << *vit << "]\n";//DEBUG lines - printing all arguments
	if (cmds_catalog.find(split_cmdline[0]) != cmds_catalog.end())
			handleCommands(fd, split_cmdline);
	else
		sendServerMessage(fd, "421", split_cmdline[0] + " :Unknown command");
}

void Server::handleCommands(int fd, const std::vector<std::string> &command)
{
	int status = _clients[fd].getStatus();

	if (status >= NOT_AUTHENTCATD && command[0] == "PASS") //store the password
		checkPass(command, fd);
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
		checkPrivmsg(command, fd);
	#if BONUS
	else if (command[0] == "VERSION")
		checkVersion();
	else if (command[0] == "STATUS")
		checkStatus(fd);
	else if (status >= REGISTRD && command[0] == "BOT") // Lulu
		checkBot(command, fd);
	else if (command[0] == "SERVER")
		checkServer(command, fd);
	else if (command[0] == "COLOR")
		checkColor(fd);
	#endif
	else
		sendServerMessage(fd, "451", ":You have not registered");
}
