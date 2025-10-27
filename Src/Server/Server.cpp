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

		// for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		// {
		// 	Channel *chan = it->second;
		// 	chan->addMember(_bot);
		// }
		for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		{
			it->second->executeJoin(*this, _bot, "");
		}

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

			if (member != sender)  // don’t send message back to sender
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

