#pragma once

#include <curl/curl.h>
#include <fstream>

#define botKeyWordActivate1 "i see you have an interest on your future, divination and the big question: who are you"
#define botKeyWordActivate2 "?\nmy generous self will give you some hints..."

#define bot0Call1 "who wakes me from my slumber ?...aaahh i see ya "
#define bot0Call2 "\nummm...there's a lot to tell"

#define bot1Call1 "tell me "
#define bot1Call2 ", do you wish to converse with me?"

#define bot2Call1 "play with me, "
#define bot2Call2 " and i will tell you something interesting"

#define botWarning0 "warning : i may be discussing with some occult forces on the great web, \
but i promise to not spill any of your secrets, anything that tells about you, \
everything stays anonymous of course"

#define botSummon "call my name 3 times if you wish to dive in the cards ?"

#define botAccept "this is me me me ⊂(灬ˊ🧿 🧿 🧿ˋ灬)⊃"

#define botSerious "talking about you"

#define botLogin "you seem to have a great attachment to numbers, i see 42...\n\
if you can tell me more about you, first will you introduce me your login:"

#define botWarning1 "warning: all infos are stored locally and erased when the server is closed."

#define botVerif ":bot bot bot"

#define botNameVerif ":D-CODER D-CODER D-CODER"

#define noBot "who are you calling..."

#define botWait "..."

#define botEnd "\r\n"

#define botBack0 "  「￣￣  「￣￣  「￣￣ "
#define botBack1 "  │⧘🧿⧙│  │⧘🧿⧙│  │⧘🧿⧙│ "
#define botBack2 "   ___」   ___」  ___」"

/*
PRIVMSG user :Please authenticate here: 
https://api.intra.42.fr/oauth/authorize?client_id=YOUR_CLIENT_ID&redirect_uri=urn:ietf:wg:oauth:2.0:oob&response_type=code
*/

//  af49ac11e76de43ff20249bc4fffebf8899c28cbee9d53c1f18927277c083ada 

// http://localhost:8080/callback

// ┌│∵│┘￣￣  │🧿│ [🧿] 〚🧿〛 「🧿」 , 『🧿』 ｢🧿｣   │⧘🧿⧙│    ⧚🧿⧛   【🧿】 , 〖🧿〗 


// curl -X POST --data "grant_type=client_credentials
// &client_id=u-s4t2ud-51b8ea7f1b5b6d799ee3947904da14f8b15b7fad5c59b5b2d85acf43d5858262
// &client_secret=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" 
// https://api.intra.42.fr/oauth/token

// curl -H "Authorization: Bearer 7ffd86542b65cb488a693ad03843b314161ee55f8a9c8ba1b1f74f6374b08938" 
// 'https://api.intra.42.fr/v2/users?filter\[login\]=lchauffo'


class Server;

class Bot
{
	private:
		std::string _clientId;//nothing shall be kept directly on the code
		std::string _clientSecret;//same here, basic security
		std::string _redirectURI;// to see what i do there
		std::string _accessToken;
	public:
		Bot();
		~Bot();
		void loadOAuthConfig();
		void refreshAccessToken();
};
