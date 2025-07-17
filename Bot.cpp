#include "Bot.hpp"
#include "Server.hpp"

Bot::Bot() : _accessToken("") { loadOAuthConfig(); }

Bot::~Bot() {}

void	Bot::initializeBot()
{
	
}

void	Bot::loadOAuthConfig()
{
	// if (!BONUS)
	// 	return ;
	std::string filename = ".apiConfig";
	std:: ifstream file(filename.c_str());
	if (!file)
	{
		std::cerr << "Cannot open config file: " << filename << std::endl;
		exit(1);
	}
	std::string tmp;
	std::getline(file, tmp);
	size_t equal = tmp.find('=', 0);
	_clientId = tmp.substr(equal);
	std::getline(file, tmp);
	equal = tmp.find('=', 0);
	_clientSecret = tmp.substr(equal);
	std::getline(file, tmp);
	equal = tmp.find('=', 0);
	_redirectURI = tmp.substr(equal);
	std::cout << "work\n";
}

size_t writeCallBack(void *contents, size_t size, size_t nmemb, void *data)
{
	size_t totalSize = size * nmemb;
	std::string *response = static_cast <std::string *>(data);
	response->append(static_cast <char *>(contents), totalSize);
	return totalSize;
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
	std::string data = begin + uid + _clientId + secret + _clientSecret + end;
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
		_accessToken = buffer;
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
