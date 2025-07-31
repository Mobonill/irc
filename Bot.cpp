#include "Bot.hpp"

Bot::Bot() : _access_token(""), _isactive(false), _server(NULL)
{
	_ip = "localghost2";
	_nickname = "ጋ-ርዐጋቹዪ";
	_username = "bot2";
	_servername = "loco2";
	_hostname = "d̵̷̰̫̮͙̝̟͖͇̞ͭ͆̐̔̾̋͐̿̔̋̑̀͋̈̾̈́ͨͨͣ̆̊̑̕̕͡͝͞ͅ-̸̢͙̰̞̮͙̦̥̤͙ͮ̉̂͒̓̇͌̐ͩ̋̒̀̒̆̂͊̒̽̍͐͟͢͟͢ͅc̨̧͚̫̲̭̮͉̀́̃̊ͨ͘͟͞ͅo̵̴̰̳͉̤̯̦͉̼͖̤̠͓ͩ̎̒ͩ̓ͭͪ̋̀ͬ̐̉ͦ͋̇ͫ̆͗̾͊d̶̸̴̸̨̧̡̡̹̫̩̲̼̯̮̮̺̺̑̆ͧ͌̾̒̅̓ͬ̆̊̊̓́ͬ̓ͭ̃͗̔̃͒̾ͨͨͯ͡͞͝e̷̝̺̞̤̫̪̗̬͚̼̜ͥͥ̌̋ͨͯ̄́̿ͩ̊ͪ̓̾̔̊ͬͧ̕͝͠r͓";
	_realname = "ጋ-ርዐጋቹዪ";
	_socket = BOT_FD;
	_status = REGISTRD;
	loadOAuthConfig();
}

Bot::Bot(Server *server) : _access_token(""), _isactive(false), _server(server)
{
	_ip = "localghost";
	_nickname = "ጋ-ርዐጋቹዪ";
	_username = "bot";
	_servername = "loco";
	_hostname = "42 d̵̷̰̫̮͙̝̟͖͇̞ͭ͆̐̔̾̋͐̿̔̋̑̀͋̈̾̈́ͨͨͣ̆̊̑̕̕͡͝͞ͅ-̸̢͙̰̞̮͙̦̥̤͙ͮ̉̂͒̓̇͌̐ͩ̋̒̀̒̆̂͊̒̽̍͐͟͢͟͢ͅc̨̧͚̫̲̭̮͉̀́̃̊ͨ͘͟͞ͅo̵̴̰̳͉̤̯̦͉̼͖̤̠͓ͩ̎̒ͩ̓ͭͪ̋̀ͬ̐̉ͦ͋̇ͫ̆͗̾͊d̶̸̴̸̨̧̡̡̹̫̩̲̼̯̮̮̺̺̑̆ͧ͌̾̒̅̓ͬ̆̊̊̓́ͬ̓ͭ̃͗̔̃͒̾ͨͨͯ͡͞͝e̷̝̺̞̤̫̪̗̬͚̼̜ͥͥ̌̋ͨͯ̄́̿ͩ̊ͪ̓̾̔̊ͬͧ̕͝͠r͓ 42";
	_realname = "ꨄ︎ ጋ-ርዐጋቹዪ ꨄ︎";
	_socket = BOT_FD;
	_status = REGISTRD;
	loadOAuthConfig();
}

Bot::~Bot() {}

void	Bot::loadOAuthConfig()
{
	std::string filename = ".apiConfig";
	std:: ifstream file(filename.c_str());
	if (!file)
	{
		std::cout << "Config file " << filename << " not found, using default values" << std::endl;
		_client_id = "default_client_id";
		_client_secret = "default_client_secret";
		_redirect_uri = "default_redirect_uri";
		return ; // Don't exit, just use defaults
		// std::cerr << "Cannot open config file: " << filename << std::endl;
		// exit(1);
	}
	std::string tmp;
	std::getline(file, tmp);
	size_t equal = tmp.find('=', 0);
	if (equal != std::string::npos)
		_client_id = tmp.substr(equal + 1);
	std::getline(file, tmp);
	equal = tmp.find('=', 0);
	if (equal != std::string::npos)
		_client_secret = tmp.substr(equal + 1);
	std::getline(file, tmp);
	equal = tmp.find('=', 0);
	if (equal != std::string::npos)
		_redirect_uri = tmp.substr(equal + 1);
	std::cout << "OAuth config loaded successfully\n";
}

size_t writeCallBack(void *contents, size_t size, size_t nmemb, void *data)
{
	size_t total_size = size * nmemb;
	std::string *response = static_cast <std::string *>(data);
	response->append(static_cast <char *>(contents), total_size);
	return total_size;
}

// thx Joachim ISACKSON

void Bot::refreshAccessToken()//call it again 
{
	CURL *curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "no work" << std::endl;
		return ;
	}
	std::string buffer;
	std::string dest = "https://api.intra.42.fr/oauth/token";
	curl_easy_setopt(curl, CURLOPT_URL, dest.c_str());
	std::string begin = "\"grant_type=client_credentials";
	std::string uid = "&client_id=";
	std::string secret = "&client_secret=";
	std::string end = "\" ";
	std::string data = begin + uid + _client_id + secret + _client_secret + end;
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cerr << "Error: no curl" << std::endl;
	}
	else
	{
		_access_token = buffer;
		std::cout << buffer << std::endl;//
	}
	curl_easy_cleanup(curl);
	return ;
}

// void	Bot::getUserInfo()
// {

// }

//co-signed by hdupire

	// std::string auth = "Authorization: Bearer ";
	// std::string dest2 = "\'https://api.intra.42.fr/v2/users?filter\\[login\\]=";
	// std::string login = _clients[clientFd].getLogin();
	// std::string close = "\'";

	// opt = " -H ";
	// std::string cmd2 = type + opt + auth + key + end + dest2 + login + close;


/*----------------------------------------------------------------------------------*/

void Bot::createBotClient()
{
	if (_isactive || !_server)
		return ;
	if (_server->createBotClient(getBotFd()))
		_isactive = true;
}

void Bot::joinChannel(const std::string &channel_name, bool &in_channel)
{
	if (!_server)
		return ;
	if (!_isactive)
		createBotClient();
	_server->addBotToChannel(channel_name, BOT_FD, in_channel);
}

void Bot::leaveChannel(const std::string &channel_name)
{
	if (!_isactive || !_server)
		return ;
	_server->removeBotFromChannel(channel_name);
}

void Bot::sendMessage(const std::string &target, const std::string &msg, bool is_channel)
{
	if (!_isactive || !_server)
		return ;
	if (is_channel)
		_server->sendMessageToChannel(target, msg);
	else
		_server->sendMessageToClient(target, msg);
}

bool Bot::isActive() const { return _isactive; }

int Bot::getBotFd() const { return BOT_FD; }

void Bot::echoClientMessage(std::string &msg, const std::string &full_client_name, int client_fd)
{
	std::string client_echo_msg;

	if (msg.empty())
		return ;
	client_echo_msg = full_client_name + " PRIVMSG " + _nickname + " " + msg + END;
	std::cout << "Echoing client message: [" << client_echo_msg << "]" << std::endl;
	send(client_fd, client_echo_msg.c_str(), client_echo_msg.size(), 0);
}

// Bot conversation methods
void Bot::handleConversation(int client_fd, const std::string &msg,\
const std::string &client_nick, const std::string & channel_name, bool in_channel)
{
	std::cout << "DEBUG: Bot::handleConversation" << std::endl;
	std::cout << "- client_nick = [" << client_nick << "]\n";
	std::string target = client_nick;
	std::cout << "- target = [" << target << "]\n";
	std::cout << "- client_fd = [" << client_fd << "]\n";
	std::cout << "- _server = [" << _server << "]\n";
	int step = _server->getClientBotStep(client_fd);
	std::cout << "- step = [" << step << "]\n";
	std::cout << "- before ifs\n";
	if (!_server)
		return ;
	if (!_isactive)
		createBotClient();
	if (channel_name.empty())
	{
		std::cout << "-- no channel == don't care\n";
		in_channel = false;
	}
	else if (channel_name[0] == '#' || channel_name[0] == '&')
	{
		std::cout << "-- looks like a channel\n";
		if (_server->addBotToChannel(channel_name, client_fd, in_channel) == false)
		{
			std::cout << "--- illegal channel interaction\n";
			step = INT_MAX;
			in_channel = false;
		}
		std::cout << "-- it's a channel\n";
	}
	else
	{
		std::cout << "-- illegal args\n";
		step = INT_MAX;
		in_channel = false;
	}
	if (!msg.empty() && msg[0] != ':')
	{
		std::cout << "-- illegal msg format\n";
		step = INT_MAX;
	}
	if (in_channel)
		target = channel_name;
	std::cout << "- after ifs\n";
	std::cout << "- target = [" << target << "]\n";
	std::cout << "- step = [" << step << "]\n";
	if (step == INT_MAX)
		stepIllegalCommand(client_fd, client_nick);
	else if (step < 0)
		stepUninvitedChannel(client_fd, client_nick, channel_name);
	else if (step == 0)
		step0(client_fd, msg, client_nick, target, in_channel);
	else if (step == 1)
		step1(client_fd, client_nick, target, in_channel);
	if (step == 2)
		step2(client_fd, client_nick, target, in_channel);
	else if (step == 3)
		step3(client_fd, msg, client_nick, target, in_channel);
	else if (step == 4)
		step4(client_fd, msg, client_nick, target, in_channel);
	std::cout << "end of function checkBot()\n";
}
