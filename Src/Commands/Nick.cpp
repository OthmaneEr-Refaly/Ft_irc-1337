/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:48:39 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/19 13:55:49 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void	handleNick(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(server, ERR_NONICKNAMEGIVEN, "NICK", "No Nickname given");
		return;
	}
	
	std::string	newNick = cmd.params[0];
	
	if (!isNickValid(newNick))
	{
		client.sendNumericReply(server, ERR_ERRONEUSNICKNAME, newNick, "Invalid Nickname");
		return ;
	}
	
	std::string normNick = normalizeCase(newNick);

	if (server.isNicknameInUse(normNick))
	{
		client.sendNumericReply(server, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
    	return;
	}

	if (!client.getNick().empty())
	{
		std::string oldNormNick = normalizeCase(client.getNick());
		server.unregisterNickname(oldNormNick);
	}
	
	std::string oldNick = client.getNick();
	client.setNick(newNick);
	server.registerNickname(normNick, &client);
	
	if (client.isRegistered())
	{
		std::string nickMsg = ":" + oldNick + "!" + client.getUser() + "@" + client.getHost()
		+ " NICK :" + newNick + "\r\n";
		
		server.sendMsgToClient(&client, nickMsg);
		const std::set<std::string>& channels = client.getChannels();
		for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		{
			Channel *chan = server.getChannel(*it);
			if (!chan)
				continue;

			const std::set<Client*>& members = chan->getMembers();
			for (std::set<Client*>::const_iterator mit = members.begin(); mit != members.end(); ++mit)
			{
				if (*mit != &client)
					server.sendMsgToClient(*mit, nickMsg);
			}
		}
	}
	else
		server.tryRegister(client);	
}
