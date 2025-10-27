#include "../../Includes/Bot.hpp"


Bot::Bot()
{
	setRegistered(true) ; 	// True if PASS, NICK, USER completed
	setPassOk(true);		// True if PASS is set correctly
	setNick("bot"); 			// NICK command value
	setUser("bot"); 			// USER command value
	setRealname("user_bot"); 		// USER command value real name field
	setHost("localhost");
	setFd(-1); 
}



Bot::~Bot(){}

void Bot::respondToMessage(Server &server, Client &sender, const std::string &message, const std::string &target)
{
    std::cout << RED<<"DEBUG: Bot responding to message: " << message << " from " << sender.getNick() << " to " << target << RESET<<std::endl;

    std::string safeMsg = server.enforceMessageLength(message);
    std::cout << RED<<"DEBUG: Bot responding to safemsg: " << safeMsg << " from " << sender.getNick() << " to " << target <<RESET <<std::endl;

    if (safeMsg == "!hello")
    {
        // Add proper IRC prefix so HexChat displays it correctly
        std::cout << RED<<"DEBUG: Bot sending Hello reply" << RESET<<std::endl;

		std::string response;
        if (target[0] == '#' || target[0] == '&') // channel
        {
			Channel *chan = server.getChannel(target);
            if (chan)
			{
				std::cout << GREEN<<"DEBUG: Bot sending Hello reply in channel" <<RESET <<std::endl;
				response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :Hello " + sender.getNick() + "!\r\n";

                server.sendMsgToChannel(*chan, response, this); // 'this' is the bot
			}
        }
        else // private message
        {
			response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :Hello " + sender.getNick() + "!\r\n";

        std::cout << GREEN<<"DEBUG: Bot sending Hello reply private msg" <<RESET <<std::endl;
std::cout << GREEN<<"DEBUG: Bot responce Hello reply" << response <<RESET<<std::endl;
            server.sendMsgToClient(&sender, response);
        }
    }
}


