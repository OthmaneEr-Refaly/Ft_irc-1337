/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:17:16 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/21 11:07:46 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

int		Server::findPollIndex(int fd) const
{
	for (size_t i = 0; i < _pollTable.size(); ++i)
	{
		if (_pollTable[i].fd == fd) 
			return static_cast<int>(i);
	}
	return (-1);
}

void	Server::enableWriteInterest(int fd)
{
	int idx = findPollIndex(fd);
	if (idx >= 0)
		_pollTable[idx].events |= POLLOUT;
}

void	Server::disableWriteInterest(int fd)
{
	int idx = findPollIndex(fd);
	if (idx >= 0)
		_pollTable[idx].events &= ~POLLOUT;
}