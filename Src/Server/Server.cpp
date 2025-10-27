/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:32 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/07 11:36:31 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"
#include "../../Includes/Bot.hpp"


// ===== Canonical form =====
	Server::Server()
		: _port(0), _password(""), _listen_fd(-1), _running(false), _bot(NULL) {}

	Server::Server(int port, const std::string& password)
		: _port(port), _password(password), _listen_fd(-1), _running(false), _bot(NULL) {}

	Server::Server(const Server& other)
	{
		*this = other;
	}

	Server& Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			_port           = other._port;
			_password       = other._password;
			_listen_fd      = other._listen_fd;
			_pollTable      = other._pollTable;
			_running        = other._running;
			_fd_to_client   = other._fd_to_client;
			_nick_to_client = other._nick_to_client;
			_channels       = other._channels;
		}
		return *this;
	}

	Server::~Server() {
		// delete _bot;
	}

// ===== Getters =====
	int 									Server::getPort() const { return (_port); }
	const std::string&						Server::getPassword() const { return (_password); }
	const std::string&						Server::getCreationDate() const { return (_creation_date); }
	bool 									Server::isRunning() const { return (_running); }
	const std::map<int, Client*>&			Server::getFdToClient() const { return (_fd_to_client); }
	const std::map<std::string, Client*>&	Server::getNickToClient() const { return (_nick_to_client); }
	const std::map<std::string, Channel*>&	Server::getChannels() const { return (_channels); }

// ===== Setters =====
	void	Server::setRunning(bool value) { _running = value; }

	void	Server::setCreationDate()
	{
		time_t	now = time(NULL);
		char	buffer[64];
		strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S", localtime(&now));
		_creation_date = buffer;	
	}

// ===== Bot =====
	void	Server::initBot()
	{
		_bot = new Bot();
		_nick_to_client["bot"] = _bot;

		
		// for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		// {
			// it->second->executeJoin(*this, _bot, "");
		// 	bot->setFd(-1);          
		

		// _fd_to_client[-1] = _bot; 
		
		std::cout << YELLOW <<"Bot initialized :: " <<RESET << std::endl;
		// }
		
	}

	Bot*	Server::getBot() const
	{
		return _bot;
	}

	void Server::sendMsgToChannel(Channel &chan, const std::string &message, Client *sender)
	{
		const std::set<Client*>& members = chan.getMembers();
		std::cout << RED<<"DEBUG: Sending message to channel " << chan.getName() <<RESET<< std::endl;
		std::cout << GREEN<<"DEBUG: Message content: " <<RESET<< message;

		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			Client* member = *it;

			if (member != sender)
			{
				std::cout << YELLOW<<"DEBUG: Sending to member: " << member->getNick() <<RESET<< std::endl;
				sendMsgToClient(member, message);
			}
			else
			{
				std::cout <<BLUE<< "DEBUG: Skipping sender: " << member->getNick() <<RESET<< std::endl;
			}
		}
	}


void Server::connectBot()
{
   
  	// struct sockaddr_in client_addr;
	// socklen_t addr_len = sizeof(client_addr);

	// int bot_fd = accept(_listen_fd, (struct sockaddr*)&client_addr, &addr_len);
	// std::cout << "bot fd : " << bot_fd << std::endl;
	// if (bot_fd == -1)
	// {
	// 	perror("accept hna");
	// 	return;
	// }

	int bot_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bot_fd < 0)
    {
        perror("socket");
        return;
    }

    // Connect to the server (localhost)
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(bot_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        close(bot_fd);
        return;
    }

	fcntl(bot_fd, F_SETFL, O_NONBLOCK);

    // --- Get client IP/Host ---
    char host[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(serv_addr.sin_addr), host, sizeof(host)) == NULL)
    {
        perror("inet_ntop");
        strcpy(host, "unknown");
    }

	Client* new_client = new Bot(bot_fd);
	//new_client->setHost(host);

	_fd_to_client[bot_fd] = new_client;

	struct pollfd pfd;
	pfd.fd = bot_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;  
	_pollTable.push_back(pfd);

	

    std::cout << RED << "Bot manually connected as client (fd = " << bot_fd << ")" << RESET << std::endl;
}



//  void Server::connectBot()
// {
   
//     int bot_fd = 4;
    
//     // // Make sure the FD is not already in use
//     // if (_fd_to_client.find(bot_fd) != _fd_to_client.end()) {
//     //     std::cout << "FD " << bot_fd << " is already in use, finding another..." << std::endl;
//     //     bot_fd = 5;
//     //     // Or find the next available FD
//     //     while (_fd_to_client.find(bot_fd) != _fd_to_client.end()) {
//     //         bot_fd++;
//     //     }
//     // }

//     // Create the bot client - use your Bot class
//     Bot* bot_client = new Bot();
//     bot_client->setFd(bot_fd);
    
//     // Set bot properties manually (since we're not getting them from accept)
//     bot_client->setNick("bot");
//     bot_client->setUser("bot");
//     bot_client->setRealname("IRC Bot");
//     bot_client->setHost("localhost");
//     bot_client->setRegistered(true); // Mark as registered since it's internal

//     // Store the bot
//     _fd_to_client[bot_fd] = bot_client;
//     _nick_to_client["bot"] = bot_client;

//     // Add to poll table
//     struct pollfd pfd;
//     pfd.fd = bot_fd;
//     pfd.events = POLLIN;
//     pfd.revents = 0;  
//     _pollTable.push_back(pfd);

//     std::cout << RED << "Bot manually connected as client (fd = " << bot_fd << ")" << RESET << std::endl;
// }

