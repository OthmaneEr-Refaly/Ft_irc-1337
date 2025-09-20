/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit_notice.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:49:46 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/20 17:58:47 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

std::string noticeMsgFormat(Client &client, const std::string &target, const std::string &msg)
{
    std::string formattedMsg = ":" + client.getNick() + "!" 
						+ client.getUser() + "@" + client.getHost()
						+ " NOTICE " + target + " :" + msg + "\r\n";
	return (formattedMsg);
}

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

void	handleNotice(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty() || cmd.params.size() < 2)
		return ;
	
			std::string target = cmd.params[0];
	std::string msg = cmd.params[1];
	std::string formattedMsg = noticeMsgFormat(client, target, msg);
	
	if (target[0] == '#')
	{
		Channel *chan = server.getChannel(target);
		if (!chan)
			return ;

		if (!chan->isMember(&client))
			return ;

		const std::set<Client*> &members = chan->getMembers();
		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			if (*it != &client) // this condition is to not send the msg to the sender
			{
				server.sendMsgToClient(*it, formattedMsg);
			}
		}
	}

	else
	{
		std::string normNick = normalizeNick(target);
		if (!server.isNicknameInUse(normNick))
			return ;
		
		std::map<std::string, Client*>::const_iterator it = server.getNickToClient().find(normNick);
		if (it != server.getNickToClient().end())
		{
			Client *targetClient = it->second;
			server.sendMsgToClient(targetClient, formattedMsg);
		}
	}
}

