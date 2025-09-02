/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:11:29 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/02 16:26:06 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>

void Server::run() 
{
	std::cout << "inside the run function" << std::endl;
	_running = true;

	initListenSocket(); // Initialize the listening socket
	if (!_running)
		return;

	while(_running)
		handlePollEvents(); // Handle events from clients and the server
}

void Server::stop()
{
}

void Server::handlePollEvents()
{
	/*****************************************************************************/
	/* Function: handlePollEvents 												 */
	/* How it works : 												 			 */
	/*   1. Poll all fds in _poll_fds with timeout 1000ms.                       */
	/*   2. Iterate through poll results:                                        */
	/*      - Listening socket ready -> acceptNewClient()                        */
	/*      - Client fd ready for read -> handleClientRead(fd)                   */
	/*      - Client fd ready for write -> handleClientWrite(fd)                 */
	/*      - Error / disconnect -> removeClient(fd)                             */
	/* Key points:                                                               */
	/*   - Ensure _poll_fds contains listening socket.                           */
	/*   - poll() should not block indefinitely.                                 */
	/*****************************************************************************/

	if (_poll_fds.empty())
		return ;

	int timeout_ms = 1000; // wait 1000ms = 1s.
	int ready = poll(&_poll_fds[0], _poll_fds.size(), timeout_ms);

	if (ready == -1)
	{
		std::cerr << "poll failed: ";
		perror("poll");
		_running = false;
		return ;
	}

	if (ready == 0)
		return ;

	
	for (size_t i = 0; i < _poll_fds.size(); i++)
	{
		int fd = _poll_fds[i].fd;
		int result_event = _poll_fds[i].revents;
		
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
			removeClient(fd);
	}
} 