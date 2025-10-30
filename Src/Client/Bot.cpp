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
        else if (safeMsg == "!joke")
        {
                std::vector<std::string> jokes ;
                jokes.push_back("Why do programmers prefer dark mode? Because light attracts bugs!");
                jokes.push_back("Why do Java developers wear glasses? Because they can't C#!");
                jokes.push_back("There are only 10 types of people in the world: those who understand binary and those who don't.");
                jokes.push_back("I would tell you a joke about UDP... but you might not get it.");
                jokes.push_back("Why was the JavaScript developer sad? Because he didn't Node how to Express himself!");
            
            
            std::string joke = jokes[std::rand() % jokes.size()];
            std::string response;
            
            if (target[0] == '#' || target[0] == '&')
            {
                Channel *chan = server.getChannel(target);
                if (chan)
                {
                    response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :" + joke + "\r\n";
                    server.sendMsgToChannel(*chan, response, this);
                }
            }
            else
            {
                response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :" + joke + "\r\n";
                server.sendMsgToClient(&sender, response);
            }
        }
        else if (safeMsg == "!quote")
        {
            std::vector<std::string> quotes ;
                quotes.push_back("The only way to do great work is to love what you do. - Steve Jobs");
                quotes.push_back("Innovation distinguishes between a leader and a follower. - Steve Jobs");
                quotes.push_back("Stay hungry, stay foolish. - Steve Jobs");
                quotes.push_back("First, solve the problem. Then, write the code. - John Johnson");
                quotes.push_back("Any fool can write code that a computer can understand. Good programmers write code that humans can understand. - Martin Fowler");
            
            std::string quote = quotes[std::rand() % quotes.size()];
            std::string response;
            
            if (target[0] == '#' || target[0] == '&')
            {
                Channel *chan = server.getChannel(target);
                if (chan)
                {
                    response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :" + quote + "\r\n";
                    server.sendMsgToChannel(*chan, response, this);
                }
            }
            else
            {
                response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :" + quote + "\r\n";
                server.sendMsgToClient(&sender, response);
            }
        }
        else if (safeMsg == "!roast")
        {
            std::vector<std::string> roasts ;
                roasts.push_back("You're not stupid; you just have bad luck thinking!");
                roasts.push_back("If laughter is the best medicine, your face must be curing the world!");
                roasts.push_back("You bring everyone so much joy... when you leave the room!");
                roasts.push_back("I'd agree with you but then we'd both be wrong!");
                roasts.push_back("you have the perfect personality for a background character!");
                roasts.push_back("You're the reason we have comment sections in code!");
                roasts.push_back("Your programming skills are so good... said no one ever!");
            
            std::string roast = roasts[std::rand() % roasts.size()];
            std::string response;
            
            if (target[0] == '#' || target[0] == '&')
            {
                Channel *chan = server.getChannel(target);
                if (chan)
                {
                    response = ":" + getNick() + "!bot@localhost PRIVMSG " + target + " :" + roast + "\r\n";
                    server.sendMsgToChannel(*chan, response, this);
                }
            }
            else
            {
                response = ":" + getNick() + "!bot@localhost PRIVMSG " + sender.getNick() + " :" + roast + "\r\n";
                server.sendMsgToClient(&sender, response);
            }
        }
    }


