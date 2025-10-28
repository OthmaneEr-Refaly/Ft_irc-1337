#include "../../Includes/Bot.hpp"


Bot::Bot()
{
	setRegistered(true);
	setPassOk(true);	
	setNick("bot");		
	setUser("bot");
	setRealname("user_bot");		
	setHost("localhost");

}
Bot::Bot(int fd) {
        
        setPassOk(true);
        setFd(fd);
        setRegistered(true);
        setHost("localhost");
        setNick("bot");		
        setRealname("user_bot");
	    setUser("bot");
    }


Bot::~Bot(){}

void Bot::respondToMessage(Server &server, Client &sender, const std::string &message, const std::string &target)
{
    std::string safeMsg = server.enforceMessageLength(message);

    if (safeMsg == "!hello")
    {
    	std::string response;
        if (target[0] == '#' || target[0] == '&') // channel
        {
			Channel *chan = server.getChannel(target);
            if (chan)
			{
				response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :Hello " + sender.getNick() + "!\r\n";
                server.sendMsgToChannel(*chan, response, this);
			}
        }
        else
        {
			response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :Hello " + sender.getNick() + "!\r\n";
            server.sendMsgToClient(&sender, response);
        }
    }
    else if (safeMsg == "!time")
    {
        std::time_t current = std::time(0);
        std::string date = std::ctime(&current);

        date.erase(date.find('\n'));

		std::string response;
        if (target[0] == '#' || target[0] == '&')
        {
			Channel *chan = server.getChannel(target);
            if (chan)
			{
				response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :Current time : " + date.c_str()  + "\r\n";
                server.sendMsgToChannel(*chan, response, this);
			}
        }
        else
        {
			response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :Current time : " + date.c_str() + "\r\n";
            server.sendMsgToClient(&sender, response);
        }
    }
    else if (safeMsg == "!commands")
    {
		std::string response;
        std::vector<std::string> commands;
            commands.push_back(" Available Commands:");
            commands.push_back("  JOIN <#channel> - Join a channel");
            commands.push_back("  KICK <#channel> <nickname> - Remove user from channel");
            commands.push_back("  MODE <#channel> <modes> - Configure channel settings");
            commands.push_back("  PRIVMSG <target> <msg> - Send private message");
            commands.push_back("  TOPIC <#channel> - View/set channel topic");
            commands.push_back("  INVITE <user> <#channel> - Invite user to channel");
            commands.push_back("  NICK <newnick> - Change your nickname");
            commands.push_back("  PASS <password> - Set password to connect to the server");
            commands.push_back("  USER <username> <hostname> <servername> <realname> - Register yourself");
            commands.push_back("  NOTICE <target> <message> - Send a notice to a user or channel");
            commands.push_back("  PART <#channel> - Leave a channel");
            commands.push_back("  QUIT [<message>] - Disconnect from the server with optional message");
    
        if (target[0] == '#' || target[0] == '&')
        {
			Channel *chan = server.getChannel(target);
            
            if (chan)
            {
                for (size_t i = 0; i < commands.size(); ++i)
                {
                    std::string response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :" + commands[i] + "\r\n";
                    server.sendMsgToChannel(*chan, response, this);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < commands.size(); ++i)
            {
                response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick()+ " :"  + commands[i] + "\r\n";
                server.sendMsgToClient(&sender, response);
            }
        }
    }
}


