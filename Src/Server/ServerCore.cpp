/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:11:29 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/22 16:09:08 by mobouifr         ###   ########.fr       */
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
	
	initListenSocket();
	if (!_running)
		return;

	setCreationDate();
	
	while(_running)
	{
		handlePollEvents();
		
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
	std::cout << "Stopping server gracefully... \n https://www.youtube.com/watch?v=AFoIP7dPQM8" << std::endl;
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
		_running = false;
		return ;
	}

	if (ready == 0)
		return ;
	
	for (size_t i = 0; i < _pollTable.size(); i++)
	{
		int fd = _pollTable[i].fd;
		int result_event = _pollTable[i].revents;
		
		if (result_event == 0)
			continue ;

		if (fd == _listen_fd && (result_event & POLLIN))
		{
			acceptNewClient();
			continue ;
		}
		if (result_event & POLLIN)
			processClientInput(fd);
		if (result_event & POLLOUT)
			sendClientResponse(fd);
		if (result_event & (POLLERR | POLLHUP | POLLNVAL))
			disconnectClient(fd, "Connection closed");
	}
} 
