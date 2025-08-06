#include "Bot.hpp"

Bot::Bot() : _access_token(""), _isactive(false), _ishexchat(false), _server(NULL)
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

Bot::Bot(Server *server) : _access_token(""), _isactive(false), _ishexchat(false), _server(server)
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

bool	Bot::refreshAccessToken()
{
	std::string buffer;
	std::string url = "https://api.intra.42.fr/oauth/token";

	std::string data = "grant_type=client_credentials";
	data += "&client_id=" + _client_id;
	data += "&client_secret=" + _client_secret;
	buffer = makeHttpsRequests(url, data, false);
	return parseReceivedToken(buffer);
}

bool	Bot::getUserInfo(int client_fd, const std::string &login)
{
	if (_access_token.empty())
		if (!refreshAccessToken())
			return false;

	std::string buffer;
	std::string url = "https://api.intra.42.fr/v2/users?filter[login]=" + login;

	std::string auth_header = "Authorization: Bearer " + _access_token;
	buffer = makeHttpsRequests(url, auth_header, true);
	std::cout << "User Info Response: [" << buffer << "]\n";
	return parseJson(client_fd, buffer);
}

bool	Bot::parseJson(int client_fd, const std::string &token)
{
	std::map <std::string, std::string> json;
	size_t begin = token.find("{");
	if (begin == std::string::npos)
		return false;
	for (size_t i = begin + 1; i < token.size() || i < begin; i++)
	{
		std::cout << "i = " << i << std::endl;
		size_t key_start = token.find("\"", i) + 1;
		size_t key_end = token.find("\"", i + 1);
		std::cout << "key_start = " << key_start << " - key_end = " << key_end << std::endl;
		std::string key = token.substr(key_start, key_end - key_start);
		std::cout << "key = [" << key << "]\n";
		i = key_end + 1;
		std::cout << "i = " << i << std::endl;
		if (token[i] != ':')
			break ;
		size_t next_bracket = i + 1;
		if (token[next_bracket] == '[')
		{
			size_t close_bracket = next_bracket + 1;
			while (next_bracket != std::string::npos || next_bracket < close_bracket)
			{
				next_bracket = token.find("[", next_bracket + 1);
				close_bracket = token.find("]", close_bracket + 1);
			}
			i = close_bracket + 1;
			continue ;
		}
		if (token[i + 1] == '{')
		{
			size_t close_bracket = next_bracket + 1;
			while (next_bracket != std::string::npos || next_bracket < close_bracket)
			{
				next_bracket = token.find("{", next_bracket + 1);
				close_bracket = token.find("}", close_bracket + 1);
			}
			i = close_bracket + 1;
			continue ;
		}
		size_t value_start = i + 1;
		size_t value_end = token.find(",", i + 1);
		std::cout << "value_start = " << value_start << " - value_end = " << value_end << std::endl;
		std::string value = token.substr(value_start, value_end - value_start);
		std::cout << "value = [" << value << "]\n";
		json.insert(std::pair <std::string, std::string> (key, value));
		i = value_end;
		std::cout << "i = " << i << std::endl;
		std::cout << "token_size = " << token.size() << std::endl;
	}
	std::cout << "DEBUG : out of for(token)\n";
	if (json.empty())
		return false;
	std::cout << "DEBUG: printing parse json\n";
	for (std::map <std::string, std::string>::iterator mit = json.begin(); mit != json.end(); ++mit)
		std::cout << "key = [" << mit->first << "] - value = [" << mit->second << "]\n";
	UserData new_user = UserData(json);
	_user.insert(std::pair <int, UserData>(client_fd, new_user));
	return true;
}


std::string Bot::makeHttpsRequests(const std::string &url, const std::string &data, bool is_user_request)
{
	std::string buffer = "";
	CURL *curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "CURL initialization failed" << std::endl;
		return buffer;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	struct curl_slist *auth_header = NULL;
	if (is_user_request)
	{
		auth_header = curl_slist_append(auth_header, data.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, auth_header);
	}
	else
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	CURLcode res = curl_easy_perform(curl);
	if (auth_header)
		curl_slist_free_all(auth_header);
	if (res != CURLE_OK)
	{
		std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
		return buffer;
	}
	else
		std::cout << "Raw API response: " << buffer << std::endl; // For debugging
	curl_easy_cleanup(curl);
	return buffer;
}

bool	Bot::parseReceivedToken(const std::string &token)
{
	std::string token_key = "\"access_token\":\"";
	size_t token_pos = token.find(token_key);
	if (token_pos == std::string::npos)
	{
		std::cerr << "❌ Failed to find access token\n";
		return false;
	}
	token_pos += token_key.size();
	size_t token_end = token.find('\"', token_pos);
	if (token_end == std::string::npos)
	{
		std::cerr << "❌ Illegal access token\n";
		return false;
	}
	token_end -= token_pos;
	_access_token = token.substr(token_pos, token_end);
	if (_access_token.empty())
	{
		std::cerr << "❌ Failed to extract access token\n";
		return false;
	}
	std::cout << "✅ Access token extracted successfully!\n";
	return true;
}



//co-signed by hdupire

	// std::string auth = "Authorization: Bearer ";
	// std::string dest2 = "\'https://api.intra.42.fr/v2/users?filter\\[login\\]=";
	// std::string login = _clients[clientFd].getLogin();
	// std::string close = "\'";

	// opt = " -H ";
	// std::string cmd2 = type + opt + auth + key + end + dest2 + login + close;

	// curl -X POST --data "grant_type=client_credentials&client_id=MY_AWESOME_UID&client_secret=MY_AWESOME_SECRET" https://api.intra.42.fr/oauth/token

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

// void Bot::echoClientMessage(std::string &msg, const std::string &full_client_name, int client_fd)
// {
// 	std::string client_echo_msg;

// 	if (msg.empty())
// 		return ;
// 	client_echo_msg = full_client_name + " PRIVMSG " + _nickname + " " + msg + END;
// 	std::cout << "Echoing client message: [" << client_echo_msg << "]" << std::endl;
// 	send(client_fd, client_echo_msg.c_str(), client_echo_msg.size(), 0);
// }//:sender_nick!user@host PRIVMSG receiver_nick :message

// Bot conversation methods
void Bot::handleConversation(int client_fd, const std::string &msg,\
const std::string &client_nick, const std::string & channel_name, bool in_channel)
{
	std::cout << "DEBUG: Bot::handleConversation" << std::endl;
	std::string target = client_nick;
	int step = _server->getClientBotStep(client_fd);
	if (!_server)
		return ;
	if (!_isactive)
		createBotClient();
	if (channel_name.empty())
		in_channel = false;
	else if (channel_name[0] == '#' || channel_name[0] == '&')
	{
		if (_server->addBotToChannel(channel_name, client_fd, in_channel) == false)
		{
			step = INT_MAX;
			in_channel = false;
		}
	}
	else if (!(step == 0 || step == 1))
	{
		step = INT_MAX;
		in_channel = false;
	}
	if (step != 0 && step != 1 && !msg.empty() && msg[0] != ':')
		step = INT_MAX;
	if (in_channel)
		target = channel_name;
	if (step == INT_MAX)
		stepIllegalCommand(client_fd, client_nick);
	else if (step < 0)
		stepUninvitedChannel(client_fd, client_nick, channel_name);
	else if (step == 0)
		step0(client_fd, msg, client_nick);
	else if (step == 1)
		step1(client_fd, msg, client_nick, target, in_channel);
	else if (step == 2)
		step2(client_fd, client_nick, target, in_channel);
	if (step == 3)
		step3(client_fd, client_nick, target, in_channel);
	else if (step == 4)
		step4(client_fd, msg, client_nick, target, in_channel);
	else if (step == 5)
		step5(client_fd, msg, target, in_channel);
	if (step == 6)
		step6(client_fd, target, in_channel);
	std::cout << "DEBUG: end of function checkBot()\n";
}
