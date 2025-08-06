#pragma once

#define botKeyWordActivate1 "i see you have an interest on your future, divination and the big question: who are you"
#define botKeyWordActivate2 "?\nmy generous self will give you some hints..."

#define botCLientType "Are you talking from a terminal [1] or hexchat [2]?"

#define botInstructionOr " or "
#define botInstruction1 "Instruction : "
#define botInstruction2 "for a private one-on-one conversation --> "
#define botInstructTerm3 BOLD + "[BOT]" + RST
#define botInstructTerm4 BOLD + "[BOT :your message]" + RST
#define botInstrucXchat3 BOLD + "[/quote BOT]" + RST
#define botInstrucXchat4 BOLD + "[/quote BOT :your message]" + RST
#define botInstrucXchat5 BOLD + "[your message]" + RST
#define botInstruction6 ", to follow conversation in channel --> "
#define botInstructTerm7 BOLD + "[BOT #channel_name_here]" + RST
#define botInstructTerm8 BOLD + "[BOT #channel_name_here :your message]" + RST
#define botInstrucXchat7 BOLD + "[/quote BOT #channel_name_here]" + RST
#define botInstrucXchat8 BOLD + "[/quote BOT #channel_name_here :your message]" + RST
#define botInstrucXchat9 BOLD + "[BOT]" + RST
#define botInstrucXchat10 BOLD + "[BOT :your message]" + RST

#define botIllegal1 "Mind your manners, here no talk like that to me"
#define botIllegal2 "I can't help you if no speak IRC to me"
#define botIllegal3 "My dear, we had a deal and you broke it first, the voices in my head are not okay with that"

#define botChanNoAccess1 "Ayaaa... "
#define botChanNoAccess2 ", you tried to enter "
#define botChanNoAccess3 " illegally, not with me i don't have paper, no money"

#define bot0Call1 " Who wakes me from my slumber ?...aaahh i see ya "
#define bot0Call2 " ummm...there's a lot to tell"
#define bot1Call1 " Tell me "
#define bot1Call2 ", do you wish to converse with me?"
#define bot2Call1 " Play with me, "
#define bot2Call2 " and i will tell you something interesting"

#define botWarning0 "Warning: i may be discussing with some occult forces on the great web, \
but i promise to not spill any of your secrets, anything that tells about you, \
everything stays anonymous of course"
#define botWarning1 "Warning: all infos are stored locally and erased when the server is closed."

#define botSummon "Call my name 3 times if you wish to dive in the cards ?"

#define botAccept "This is me me me ⊂(灬ˊ🧿 🧿 🧿ˋ灬)⊃"
#define botLogin1 "Talking about you "
#define botLogin2 ", you seem to have a great attachment to numbers, i see 42...\
if you can tell me more about you, first will you introduce me your login:"

#define botVerif ":bot bot bot"
#define botVerifUp ":BOT BOT BOT"
#define botNameVerif ":D-CODER D-CODER D-CODER"

#define noBot "Who are you calling..."

#define botIgnore "...mmm... it's true some secret are best being unsaid \
or maybe you're not part of the sect... i mean school"

#define botOAuth "The ethereal forces reveal... Your digital aura shows great potential! Let's dive into the cards"
#define botNoAuth "The cosmic connection failed... The spirits are silent."

#define botWait "..."

#define botEnd "\r\n"

#define botBack0 "  「￣￣  「￣￣  「￣￣ "
#define botBack1 "  │⧘🧿⧙│  │⧘🧿⧙│  │⧘🧿⧙│ "
#define botBack2 "   ___」   ___」  ___」"

// std::string summonExpressions = "divination,tarot,futur,42forecast,DE-CODER,Dcoder,decoder,read me,akinator";

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
