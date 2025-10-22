/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:13:32 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/21 09:28:54 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

	Server* Server::g_instance = NULL;

// ===== Canonical form ===== (Server can't be copied or assigned)
	Server::Server()
		: _port(0), _password(""), _listen_fd(-1), _running(false) {}

	Server::Server(int port, const std::string& password)
		: _port(port), _password(password), _listen_fd(-1), _running(false) {}

	Server::~Server() {}

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

