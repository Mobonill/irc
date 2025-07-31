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
	compose_msg = RED + botChanNoAccess1 + BOLD + nick + RST + RED + botChanNoAccess2 \
	+ BOLD + illegal_channel + RST + RED + botChanNoAccess3 + RST;
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
	compose_msg = LBU + botInstruction1 + BOLD + botInstruction2 + RST \
	+ LBU + botInstruction3 + BOLD + botInstruction4 + RST + LBU + botInstruction5 \
	+ BOLD + botInstruction6 + RST + LBU + botInstruction7 + RST;
	sendMessage(nick, compose_msg, false);
	std::srand(std::time(0));
	const int randthree = std::rand() % 3;
	if (randthree == 0)
		compose_msg = PPL + bot0Call1 + BOLD + nick + RST + PPL + bot0Call2 + RST;
	else if (randthree == 1)
		compose_msg = PPL + bot1Call1 + BOLD + nick + RST + PPL + bot1Call2 + RST;
	else if (randthree == 2)
		compose_msg = PPL + bot2Call1 + BOLD + nick + RST + PPL + bot2Call2 + RST;
	std::cout << "compose_msg = [" << compose_msg << "]\n";
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 2);
}

void	Bot::step1(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//here, client summon BOT indirectly, through keyword detection
{
	std::string compose_msg;

	std::cout << "-- entering bot [1]" << std::endl;
	compose_msg = PPL + botKeyWordActivate1 + nick + botKeyWordActivate2 + RST;
	sendMessage(target, compose_msg, in_channel);
	_server->setClientBotStep(client_fd, 2);
}

void	Bot::step2(const int &client_fd, const std::string &nick, const std::string &target, bool in_channel)//follow with warning and confirmation demand
{
	std::string compose_msg;
	std::string bmsg;

	std::cout << "-- entering bot [2]" << std::endl;
	compose_msg = ITALIC + BOLD + RED + botWarning0 + RST;
	sendMessage(nick, compose_msg, false);
	compose_msg = PPL + botSummon + RST;
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
		compose_msg = PPL + botWait + RST;
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
			compose_msg = PPL + botAccept + RST;
			sendMessage(target, compose_msg, in_channel);
			compose_msg = ITALIC + BOLD + RED + botWarning1 + RST;
			sendMessage(nick, compose_msg, false);
			compose_msg = PPL + botLogin1 + BOLD + nick + RST + PPL + botLogin2 + RST;
			sendMessage(target, compose_msg, in_channel);
			_server->setClientBotStep(client_fd, 4);
		}
		else
		{
			compose_msg = BRW + noBot + msg+ RST;
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
		compose_msg = PNK + botWait + RST;
		sendMessage(target, compose_msg, in_channel);
		_server->setClientBotStep(client_fd, 0);
	}//botIgnore when the api fail
	else// get API to check
	{
		std::cout << "*** End of bot conv!! ***\n";
		return ;
	}
}