/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:32 by mobouifr          #+#    #+#             */
/*   Updated: 2025/11/04 09:02:48 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"
#include "../../Includes/Bot.hpp"


// ===== Canonical form =====
	Server::Server()
		: _port(0), _password(""), _listen_fd(-1), _running(false), _bot(NULL) {}

	Server::Server(int port, const std::string& password)
		: _port(port), _password(password), _listen_fd(-1), _running(false), _bot(NULL) {}

	Server::~Server() {
		delete _bot;
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
	}

	Bot*	Server::getBot() const
	{
		return _bot;
	}

	void Server::sendMsgToChannel(Channel &chan, const std::string &message, Client *sender)
	{
		const std::set<Client*>& members = chan.getMembers();

		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			Client* member = *it;

			if (member != sender)
			{
				std::cout << YELLOW<<"DEBUG: Sending to member: " << member->getNick() <<RESET<< std::endl;
				sendMsgToClient(member, message);
			}
		}
	}

	// void	Server::connectBot()
	// {

	// 	int bot_fd = socket(AF_INET, SOCK_STREAM, 0);
	// 	if (bot_fd < 0)
	// 	{
	// 		perror("socket");
	// 		return;
	// 	}

	// 	// Connect to the server (localhost)
	// 	struct sockaddr_in serv_addr;
	// 	serv_addr.sin_family = AF_INET;
	// 	serv_addr.sin_port = htons(_port);
	// 	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	// 	if (connect(bot_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	// 	{
	// 		perror("connect");
	// 		close(bot_fd);
	// 		return;
	// 	}

	// 	fcntl(bot_fd, F_SETFL, O_NONBLOCK);

	// 	// --- Get client IP/Host ---
	// 	char host[INET_ADDRSTRLEN];
	// 	if (inet_ntop(AF_INET, &(serv_addr.sin_addr), host, sizeof(host)) == NULL)
	// 	{
	// 		perror("inet_ntop");
	// 		strcpy(host, "unknown");
	// 	}

	// 	Client* new_client = new Bot(bot_fd);

	// 	_fd_to_client[bot_fd] = new_client;
		
		

	// 	struct pollfd pfd;
	// 	pfd.fd = bot_fd;
	// 	pfd.events = POLLIN;
	// 	pfd.revents = 0;  
	// 	_pollTable.push_back(pfd);

	// }
