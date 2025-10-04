/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:11:29 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 15:30:00 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Server::run() 
{
	std::cout << "inside the run function" << std::endl;
	_running = true;

	g_instance = this;
	signal(SIGINT, Server::handleSignal);
	signal(SIGTERM, Server::handleSignal);
	
	initListenSocket(); // Initialize the listening socket
	if (!_running)
		return;

	time_t	now = time(NULL);
	char	buffer[64];
	strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S", localtime(&now));
	_creation_date = buffer;
	
	while(_running)
	{
		handlePollEvents(); // Handle events from clients and the Server
		
		// loop to check if there is client that needs to be removed.
		for (std::map<int, Client*>::iterator it = _fd_to_client.begin(); it != _fd_to_client.end(); )
		{
			Client *client = it->second;
			if(client->isClosing())
			{
				++it;
				removeClient(client->getFd());
			}
			else
				++it;
		}
	}
}

void Server::stop()
{
	std::cout << "Stopping server gracefully..." << std::endl;
	_running = false;

	if (_listen_fd != -1)
	{
		close(_listen_fd);
		_listen_fd = -1;
	}

	for (std::map<int, Client*>::iterator it = _fd_to_client.begin(); it != _fd_to_client.end(); )
	{
		Client *client = it->second;
		++it; // increment before disconnect
		disconnectClient(client->getFd(), "Server shutting down");
	}

	_pollTable.clear();
	_fd_to_client.clear();
	_nick_to_client.clear();
	_channels.clear();
}

void Server::handlePollEvents()
{

	if (_pollTable.empty())
		return ;

	int timeout_ms = 1000; // wait 1000ms = 1s.
	int ready = poll(&_pollTable[0], _pollTable.size(), timeout_ms);

	if (ready == -1)
	{
		std::cerr << "poll failed: ";
		perror("poll");
		_running = false;
		return ;
	}

	if (ready == 0)
		return ;

	
	for (size_t i = 0; i < _pollTable.size(); i++)
	{
		int fd = _pollTable[i].fd;
		int result_event = _pollTable[i].revents;
		
		if (result_event == 0) // no events.
			continue ;

		if (fd == _listen_fd && (result_event & POLLIN)) // 1. New connection on listening socket
		{
			acceptNewClient();
			continue ;
		}
		if (result_event & POLLIN) 	// 2. Existing client sent data
			handleClientRead(fd);
		if (result_event & POLLOUT) // 3. Existing client ready to send
			handleClientWrite(fd);
		if (result_event & (POLLERR | POLLHUP | POLLNVAL)) // 4. Error or disconnection
			disconnectClient(fd, "Connection closed");
	}
} 
