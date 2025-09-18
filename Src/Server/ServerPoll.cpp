/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:17:16 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/12 11:28:20 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>

int		Server::findPollIndex(int fd) const
{
	/*****************************************************************************/
	/* Function: findPollIndex                                                   */
	/* How it works:                                                             */
	/*   1. Iterate through _pollTable vector.                                    */
	/*   2. Compare each fd with the input fd.                                   */
	/*   3. Return index if found, -1 if not found.                              */
	/* Key points:                                                               */
	/*   - Used internally to enable/disable POLLOUT for specific fd.            */
	/*****************************************************************************/

	for (size_t i = 0; i < _pollTable.size(); ++i)
	{
		if (_pollTable[i].fd == fd) 
			return static_cast<int>(i);
	}
	return (-1);
}

void	Server::enableWriteInterest(int fd)
{
	/*****************************************************************************/
	/* Function: enableWriteInterest                                             */
	/* How it works:                                                             */
	/*   1. Find fd index in _pollTable using findPollIndex().                    */
	/*   2. Set POLLOUT bit in events for that fd.                               */
	/* Key points:                                                               */
	/*   - Allows handleClientWrite to be triggered when fd is writable.         */
	/*****************************************************************************/

	int idx = findPollIndex(fd);
	if (idx >= 0)
		_pollTable[idx].events |= POLLOUT;
}

void	Server::disableWriteInterest(int fd)
{
	/*****************************************************************************/
	/* Function: disableWriteInterest                                            */
	/* How it works:                                                             */
	/*   1. Find fd index in _pollTable using findPollIndex().                    */
	/*   2. Clear POLLOUT bit in events for that fd.                             */
	/* Key points:                                                               */
	/*   - Stops unnecessary POLLOUT events once all data has been sent.         */
	/*****************************************************************************/

	int idx = findPollIndex(fd);
	if (idx >= 0)
		_pollTable[idx].events &= ~POLLOUT;
}