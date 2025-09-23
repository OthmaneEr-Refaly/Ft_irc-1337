/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:40:45 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 15:57:00 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

std::string quitMsgFormat(Client &client, const std::string &reason)
{
	std::string formattedMsg = ":" + client.getNick() + "!" + client.getUser()
					+ "@" + client.getHost() + " QUIT :" + reason + "\r\n";

	return formattedMsg;
}

void	handleQuit(Server &server, Client &client, const Command &cmd)
{
	std::string reason = "Quit";
	
	if (!cmd.params.empty())
		reason = cmd.params[0];
	
	std::string formattedMsg = quitMsgFormat(client, reason);
	
	const std::set<std::string> &channels = client.getChannels();
	for (std::set<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel * channel = server.getChannel(*it);
		if (channel)
		{
			const std::set<Client*> &members = channel->getMembers();
			for (std::set<Client*>::const_iterator mit = members.begin(); mit != members.end(); ++mit)
			{
				if (*mit != &client)
					server.sendMsgToClient(*mit, formattedMsg);
			}
		}
	}
	server.disconnectClient(client.getFd(), reason);
}
