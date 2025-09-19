/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:21:56 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/19 09:00:51 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

std::string privmsgFormat(Client &client, const std::string &target, const std::string &msg)
{
    std::string fullMsg = ":" + client.getNick() + "!" 
						+ client.getUser() + "@" + client.getHost()
						+ " PRIVMSG " + target + " :" + msg + "\r\n";
	return (fullMsg);
}

void	handlePrivmsg(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(ERR_NORECIPIENT, "PRIVMSG", "No recipient given");
		return ;
	}

	if (cmd.params.size() < 2)
	{
		client.sendNumericReply(ERR_NOTEXTTOSEND, cmd.params[0], "No text to send");
		return ;
	}

	std::string target = cmd.params[0];
	std::string msg = cmd.params[1];
	std::string formattedMsg = privmsgFormat(client, target, msg);
	
	if (target[0] == '#')
	{
		
	}

	else
	{
		std::string normNick = normalizeNick(target);
		if (!server.isNicknameInUse(normNick))
		{
			client.sendNumericReply(ERR_NOSUCHNICK, target, "No such nick/channel");
			return ;
		}
		std::map<std::string, Client*>::const_iterator it = server.getNickToClient().find(normNick);
		if (it != server.getNickToClient().end())
		{
			Client *targetClient = it->second;
			server.sendMsgToClient(targetClient, formattedMsg);
		}
	}
}
