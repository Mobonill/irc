#include "Bot.hpp"

void	Bot::stepIllegalCommand(const int &client_fd, const std::string &nick)
{
	std::string compose_msg;

	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	if (randthree == 0)
		compose_msg = RED + botIllegal1 + RST;
	else if (randthree == 1)
		compose_msg = RED + botIllegal2 + RST;
	else if (randthree == 2)
		compose_msg = RED + botIllegal3 + RST;
	sendMessage(nick, compose_msg, false);
	_server->setClientBotStep(client_fd, 0);
	_server->sendServerMessage(client_fd, "421", "BOT :Unknown command");
}

void	Bot::stepUninvitedChannel(const int &client_fd, const std::string &nick, const std::string &illegal_channel)
{
	std::string compose_msg;
	compose_msg = RED + botChanNoAccess1 + nick + botChanNoAccess2 + illegal_channel + botChanNoAccess3 + RST;
	sendMessage(nick, compose_msg, false);
	_server->setClientBotStep(client_fd, -1);
	_server->sendServerMessage(client_fd, "442", illegal_channel + " :You're not on that channel");
}

void	Bot::step0(const int &client_fd, const std::string &msg, const std::string &nick, \
const std::string &target, bool in_channel)//here, client summon BOT directly
{
	std::string compose_msg;

	std::cout << "-- entering bot [0]" << std::endl;
	if (!msg.empty() && msg[0] != ':')
		return _server->sendServerMessage(client_fd, "421", "BOT :Unknown command");
	compose_msg = LBLUE + botInstruction1 + BOLD + botInstruction2 + RST + LBLUE + botInstruction3 + BOLD + botInstruction4 + RST \
	+ LBLUE + botInstruction5 + BOLD + botInstruction6 + RST + LBLUE + botInstruction7 + RST;
	sendMessage(nick, compose_msg, false);
	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	if (randthree == 0)
		compose_msg = PINK + bot0Call1 + nick + bot0Call2 + RST;
	else if (randthree == 1)
		compose_msg = PINK + bot1Call1 + nick + bot1Call2 + RST;
	else if (randthree == 2)
		compose_msg = PINK+ bot2Call1 + nick + bot2Call2 + RST;
	std::cout << "compose_msg = [" << compose_msg << "]\n";
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 2);
}

void	Bot::step1(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//here, client summon BOT indirectly, through keyword detection
{
	std::string compose_msg;

	std::cout << "-- entering bot [1]" << std::endl;
	compose_msg = BLU + botKeyWordActivate1 + nick + botKeyWordActivate2 + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 2);
}

void	Bot::step2(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//follow with warning and confirmation demand
{
	std::string compose_msg;
	std::string bmsg;

	std::cout << "-- entering bot [2]" << std::endl;
	compose_msg = ITALIC + BOLD + "\x03\14" + botWarning0 + RST;
	sendMessage(nick, compose_msg, false);
	compose_msg = RDM + botSummon + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 3);
}

void	Bot::step3(const int &client_fd, std::string msg, const std::string &nick, \
const std::string &target, bool in_channel)//client confirms bot bot bot
{
	std::string compose_msg;
	std::string bmsg;

	std::cout << "-- entering bot [3]" << std::endl;
	if (msg.empty())
	{
		compose_msg = BLU + botWait + RST;
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
			compose_msg = PINK + botAccept + RST;
			sendMessage(target, compose_msg, in_channel);
			compose_msg = PINK + botSerious + RST;
			sendMessage(target, compose_msg, in_channel);
			compose_msg = PINK + botLogin + RST;
			sendMessage(target, compose_msg, in_channel);
			compose_msg = ITALIC + BOLD + "\x03\01" + botWarning1 + RST;
			sendMessage(nick, compose_msg, false);
			_server->setClientBotStep(client_fd, 4);
		}
		else
		{
			compose_msg = RDM + noBot + msg+ RST;
			sendMessage(target, compose_msg, in_channel);
			_server->setClientBotStep(client_fd, 0);
		}
	}
}

void	Bot::step4(const int &client_fd, std::string login, const std::string &nick, \
const std::string &target, bool in_channel)//get the login here
{
	std::string compose_msg;
	std::string bmsg;

	std::cout << "-- entering bot [4]" << std::endl;
	(void)nick;
	if (login.empty())
	{
		compose_msg = BLU + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		return _server->setClientBotStep(client_fd, 0);//skip step
	}
	int end = (int)login.size() - 1;
	int spaces = end;
	while (login[spaces] && std::isspace(login[spaces]))
		--spaces;
	if (spaces != end)
		login.erase(spaces + 1, end);
	login.erase(0, 1);
	std::cout << "login : [" << login << "]\n";
	if (login.find(" ") != std::string::npos)
	{
		compose_msg = PINK + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		_server->setClientBotStep(client_fd, 0);
	}//botIgnore when the api fail
	else// get API to check
	{
		std::cout << "*** End of bot conv!! ***\n";
		return ;
	}
}