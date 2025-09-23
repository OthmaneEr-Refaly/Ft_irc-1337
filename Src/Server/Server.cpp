// #include "../Includes/Server.hpp"
// #include "../Includes/Client.hpp"
// #include "../Includes/Channel.hpp"
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <arpa/inet.h>
// #include <sys/types.h>


#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/NumericReplies.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>

// ===== Canonical form =====
	Server::Server()
		: _port(0), _password(""), _listen_fd(-1), _running(false) {}

	Server::Server(int port, const std::string& password)
		: _port(port), _password(password), _listen_fd(-1), _running(false) {}

	Server::Server(const Server& other)
	{
		*this = other;
	}

	Server& Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			_port          = other._port;
			_password      = other._password;
			_listen_fd     = other._listen_fd;
			_pollTable     = other._pollTable;
			_running       = other._running;
			_fd_to_client  = other._fd_to_client;
			_nick_to_client= other._nick_to_client;
			_channels      = other._channels;
		}
		return *this;
	}

	Server::~Server() {}

// ===== Getters =====
	int 									Server::getPort() const { return (_port); }
	const std::string&						Server::getPassword() const { return (_password); }
	bool 									Server::isRunning() const { return (_running); }
	const std::map<int, Client*>&			Server::getFdToClient() const { return (_fd_to_client); }
	const std::map<std::string, Client*>&	Server::getNickToClient() const { return (_nick_to_client); }
	const std::map<std::string, Channel*>&	Server::getChannels() const { return (_channels); }

// ===== Setters =====
	void Server::setRunning(bool value) { _running = value; }

// ===== [MB] empty title ======
	void Server::disconnectClient(int fd, const std::string &reason)
	{
		std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
		if (it == _fd_to_client.end())
			return ;
		
		Client *client = it->second;

		if(!reason.empty())
		{
			std::string msg = ":" + client->getNick() + " QUIT :" + reason + "\r\n";
			sendToFd(fd, msg); // sendToFd() function still not implemented.
		}
		client->markForClose();

		std::cout << "marked Client Fd=" << fd << " for close ( reason: " << reason << " )" << std::endl;
	}	

	void	Server::tryRegister(Client &client)
	{
		if (client.isRegistered())
			return ;

		if (!client.isPassOk())
			return ;
		if (client.getNick().empty())
			return ;
		if (client.getUser().empty() || client.getRealname().empty())
			return;

		client.setRegistered(true);

		client.sendNumericReply(RPL_WELCOME, client.getNick(),
        "Welcome to the IRC network " + client.getNick() + "!" + client.getUser() + "@" + client.getHost());

		client.sendNumericReply(RPL_YOURHOST, client.getNick(),
        "Your host is ft_irc, running version sma9ma9");

	    client.sendNumericReply(RPL_CREATED, client.getNick(),
        "This server was created <date>");

	    client.sendNumericReply(RPL_MYINFO, client.getNick(),
        "ft_irc sam9ma9aw(1.0) o o"/*message:"Server name, version, supported user & channel modes"*/);  
	}

// ===== [MB] _nick_to_client map helper functions =====
	void	Server::registerNickname(const std::string &nick, Client *client)
	{
		_nick_to_client[nick] = client;
	}

	void	Server::unregisterNickname(const std::string &nick)
	{
		_nick_to_client.erase(nick);
	}

	bool	Server::isNicknameInUse(const std::string &nick) const
	{
		return (_nick_to_client.count(nick) > 0);
	}

	Client*	Server::findClientByNick(const std::string &nick)
	{
		for (std::map<std::string, Client*>::iterator it = _nick_to_client.begin(); it != _nick_to_client.end(); ++it)
		{
			if (it->second->getNick() == nick)
			{
				return (it->second);
			}
		}
		return (NULL);
	}


// ===== [MB] PRIVMSG helper functions =====
	void	Server::sendMsgToClient(Client *client, const std::string &msg)
	{
		std::string safeMsg = enforceMessageLength(msg);
		client->enqueueOutput(safeMsg);
		enableWriteInterest(client->getFd());
	}

	std::string Server::enforceMessageLength(const std::string &rawMessage)
	{
		if (rawMessage.size() <= IRC_MAX_LINE)
			return (rawMessage);

		size_t payloadStart = rawMessage.find(" :");
		if (payloadStart != std::string::npos)
		{
			std::string headerPart = rawMessage.substr(0, payloadStart + 2); // +2 for the " :"
			std::string payloadPart = rawMessage.substr(payloadStart + 2);

			size_t headerPartSize = headerPart.size() + 2; // +2 this time is for the upcoming \r\n
			if (headerPartSize >=IRC_MAX_LINE)
				return (rawMessage.substr(0, IRC_MAX_CONTENT) + "\r\n");
			
			size_t maxPayloadLength = IRC_MAX_LINE - headerPartSize;
			if (payloadPart.size() > maxPayloadLength)
				payloadPart = payloadPart.substr(0, maxPayloadLength);
				
			return (headerPart + payloadPart + "\r\n");
		}

		return (rawMessage.substr(0, IRC_MAX_CONTENT) + "\r\n");
	}

	Channel* Server::getChannel(const std::string& channelName) 
	{
	    std::cout << "Looking for channel: " << channelName << std::endl;
	    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	    if (it != _channels.end()) {
	        std::cout << "Channel found: " << channelName << std::endl;
	        return it->second;
	    }
	    std::cout << "Channel not found: " << channelName << std::endl;
	    return NULL;
	}

	Channel* Server::createChannel(const std::string& channelName)
	{
	    std::cout << "debug Creating new channel: " << channelName << std::endl;
	    Channel* newChannel = new Channel(channelName);
	    _channels[channelName] = newChannel;
	    std::cout << "debab Channel created and added to map: " << channelName << std::endl;
	    return newChannel;
	}

	void Server::removeChannel(const std::string& channelName)
	{
		std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
		if (it != _channels.end())
		{
			delete it->second;
			_channels.erase(it);
			std::cout << "[debug] channel removed : " << channelName << std::endl;
		}
	}