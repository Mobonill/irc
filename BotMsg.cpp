#include "Bot.hpp"

void	Bot::stepIllegalCommand(const int &client_fd, const std::string &nick)
{
	std::string compose_msg;
	std::string color;

	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	color = _ishexchat ? RED : "";
	if (randthree == 0)
		compose_msg = color + botIllegal1 + RST;
	else if (randthree == 1)
		compose_msg = color + botIllegal2 + RST;
	else if (randthree == 2)
		compose_msg = color + botIllegal3 + RST;
	sendMessage(nick, compose_msg, false);
	_server->setClientBotStep(client_fd, 0);
	_server->sendServerMessage(client_fd, "421", "BOT :Unknown command");
}

void	Bot::stepUninvitedChannel(const int &client_fd, const std::string &nick, const std::string &illegal_channel)
{
	std::string compose_msg;
	std::string color;
	color = _ishexchat ? RED : "";
	compose_msg = color + botChanNoAccess1 + BOLD + nick + RST + color + botChanNoAccess2 \
	+ BOLD + illegal_channel + RST + color + botChanNoAccess3 + RST;
	sendMessage(nick, compose_msg, false);
	_server->setClientBotStep(client_fd, -1);
	_server->sendServerMessage(client_fd, "442", illegal_channel + " :You're not on that channel");
}

void	Bot::step0(const int &client_fd, const std::string &msg, const std::string &nick)
{
	std::string compose_msg = "";

	std::cout << "-- entering bot [0]" << std::endl;
	std::cout << "DEBUG: msg = [" << msg << "]\n";
	if (!msg.empty() && msg[0] != ':')
		return _server->sendServerMessage(client_fd, "421", "BOT :Unknown command");
	compose_msg = botCLientType;
	sendMessage(nick, compose_msg, false);
	_server->setClientBotStep(client_fd, 1);
}

const std::string	Bot::stepInstructionMsg()
{
	std::string instruction = "";
	std::string color;

	color = _ishexchat ? YLW : "";
	if (_ishexchat)
		instruction = color + botInstruction1 + botInstruction2 + botInstrucXchat3 + color + botInstructionOr \
		+ botInstrucXchat4 + color + botInstructionOr +botInstrucXchat5 + color + botInstruction6 + botInstrucXchat7\
		+ color + botInstructionOr + botInstrucXchat8 + color + botInstructionOr + botInstrucXchat9 \
		+ color + botInstructionOr + botInstrucXchat10;
	else
		instruction = std::string(botInstruction1) + botInstruction2 + botInstructTerm3 + botInstructionOr \
		+ botInstructTerm4 + botInstruction6 + botInstructTerm7 + botInstructionOr + botInstructTerm8;
	return instruction;
}

void	Bot::step1(const int &client_fd, const std::string &msg, const std::string &nick, \
const std::string &target, bool in_channel)//here, client summon BOT directly
{
	std::string compose_msg;
	std::string color;

	std::cout << "-- entering bot [1]" << std::endl;
	if (!msg.empty() && msg.compare(":2") == 0)
		_ishexchat = true;
	else if (!msg.empty() && msg.compare(":1") == 0)
		_ishexchat = false;
	else
	{
		std::cout << "DEBUG: step1 failed - msg = [" << msg << "]\n";
		return _server->sendServerMessage(client_fd, "421", "BOT :Unknown command");
	}
	compose_msg = stepInstructionMsg();
	sendMessage(nick, compose_msg, false);
	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	color = _ishexchat ? PPL : "";
	if (randthree == 0)
		compose_msg = color + bot0Call1 + BOLD + nick + RST + color + bot0Call2 + RST;
	else if (randthree == 1)
		compose_msg = color + bot1Call1 + BOLD + nick + RST + color + bot1Call2 + RST;
	else if (randthree == 2)
		compose_msg = color + bot2Call1 + BOLD + nick + RST + color + bot2Call2 + RST;
	std::cout << "compose_msg = [" << compose_msg << "]\n";
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 3);
}

void	Bot::step2(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//here, client summon BOT indirectly, through keyword detection
{
	std::string compose_msg;
	std::string color = "";

	std::cout << "-- entering bot [2]" << std::endl;
	color = _ishexchat ? PPL : "";
	compose_msg = color + botKeyWordActivate1 + nick + botKeyWordActivate2 + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 3);
}

void	Bot::step3(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//follow with warning and confirmation demand
{
	std::string compose_msg;
	std::string bmsg;
	std::string color = "";

	std::cout << "-- entering bot [3]" << std::endl;
	color = _ishexchat ? RED : "";
	compose_msg = color + ITALIC + BOLD + botWarning0 + RST;
	sendMessage(nick, compose_msg, false);
	color = _ishexchat ? PPL : "";
	compose_msg = color + botSummon + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 4);
}

void	Bot::step4(const int &client_fd, std::string msg, const std::string &nick, \
const std::string &target, bool in_channel)//client confirms bot bot bot
{
	std::string compose_msg;
	std::string bmsg;
	std::string color;

	std::cout << "-- entering bot [4]" << std::endl;
	color = _ishexchat ? PPL : "";
	if (msg.empty())
	{
		compose_msg = color + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		_server->setClientBotStep(client_fd, 0);
	}
	else
	{
		int end = (int)msg.size() - 1;
		int spaces = end;
		while (msg[spaces] && std::isspace(msg[spaces]))
			--spaces;
		if (spaces != end)
			msg.erase(spaces + 1, end);
		if (msg.compare(botVerif) == 0 || msg.compare(botVerifUp) == 0 \
		|| msg.compare(botNameVerif) == 0)
		{
			compose_msg = color + botAccept + RST;
			sendMessage(target, compose_msg, in_channel);
			color = _ishexchat ? RED : "";
			compose_msg = color + ITALIC + BOLD + botWarning1 + RST;
			sendMessage(nick, compose_msg, false);
			color = _ishexchat ? PPL : "";
			compose_msg = color + botLogin1 + BOLD + nick + RST + color + botLogin2 + RST;
			sendMessage(target, compose_msg, in_channel);
			_server->setClientBotStep(client_fd, 5);
		}
		else
		{
			color = _ishexchat ? BRW : "";
			compose_msg = color + noBot + msg+ RST;
			sendMessage(target, compose_msg, in_channel);
			_server->setClientBotStep(client_fd, 0);
		}
	}
}

void	Bot::step5(const int &client_fd, std::string login, const std::string &target, bool in_channel)//get the login here
{
	std::string compose_msg;
	std::string bmsg;
	std::string color;

	std::cout << "-- entering bot [5]" << std::endl;
	if (login.empty())
	{
		color = _ishexchat ? BLU : "";
		compose_msg = color + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		return _server->setClientBotStep(client_fd, 0);
	}
	int end = (int)login.size() - 1;
	int spaces = end;
	while (login[spaces] && std::isspace(login[spaces]))
		--spaces;
	if (spaces != end)
		login.erase(spaces + 1, end);
	login.erase(0, 1);
	std::cout << "DEBUG: login = [" << login << "]\n";
	if (login.find(" ") != std::string::npos || !onlyValid(login, ALPHA + ALPHA_UP))
	{
		color = _ishexchat ? PPL : "";
		compose_msg = color + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		_server->setClientBotStep(client_fd, 0);
	}
	else
	{
		if (refreshAccessToken())
		{
			std::cout << "*** API process initiated successfully ***\n";
			if (getUserInfo(client_fd, login))
			{
				color = _ishexchat ? PPL : "";
				compose_msg = color + botOAuth + RST;
				sendMessage(target, compose_msg, in_channel);
			}
			else
			{
				color = _ishexchat ? RED : "";
				compose_msg = color + botNoAuth + RST;
				sendMessage(target, compose_msg, in_channel);
			}
		}
		else
		{
			std::cerr << "Failed to refresh access token" << std::endl;
			std::cout << "*** Authentication failed ***\n";
			color = _ishexchat ? PPL : "";
			compose_msg = color + botIgnore + RST;
			sendMessage(target, compose_msg, in_channel);
		}
		std::cout << "going to step6\n";
		_server->setClientBotStep(client_fd, 6);
		return ;
	}
}

void	Bot::step6(const int &client_fd, const std::string &target, bool in_channel)
{
	std::string compose_msg;
	std::string bmsg;
	std::string color = "";

	std::cout << "-- entering bot [6]" << std::endl;
	color = _ishexchat ? PPL : "";
	compose_msg = color + botBack0 + RST;
	sendMessage(target, compose_msg, in_channel);
	compose_msg = color + botBack1 + RST;
	sendMessage(target, compose_msg, in_channel);
	compose_msg = color + botBack2 + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 0);
	std::cout << "*** DEBUG: End of bot conv!! ***\n";
}
