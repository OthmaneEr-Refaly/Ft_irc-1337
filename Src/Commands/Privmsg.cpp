/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:21:56 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/19 14:37:29 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    PRIVMSG Command Handler
    -----------------------
    - Implements sending messages between users and to channels.
    - RFC Rules:
        • A message must always have a recipient and text.
        • Targets can be nicknames, channels, or a comma-separated list of both.
        • Messages longer than 512 bytes (including CRLF) must be truncated.
        • When sending to a channel, the message is broadcast to all members 
          except the sender.
        • If a target nick/channel does not exist → ERR_NOSUCHNICK or ERR_NOSUCHCHANNEL.
        • If the sender is not on the channel → ERR_NOTONCHANNEL.
    - Helpers:
        • privmsgFormat() → Formats messages as ":nick!user@host PRIVMSG target :message".
        • Server::sendMsgToClient() → Handles safe delivery to a single client.
*/

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

void	handlePrivmsg(Server &server, Client &client, const Command &cmd) // the case of multipe target should be also set later
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(ERR_NORECIPIENT, "PRIVMSG", "No recipient given (PRIVMSG)");
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
		Channel *chan = server.getChannel(target);
		if (!chan)
		{
			client.sendNumericReply(ERR_NOSUCHCHANNEL, target, "No Such channel");
			return ;
		}

		if (!chan->isMember(&client))
		{
			client.sendNumericReply(ERR_NOTONCHANNEL, target, "You're not on that channel");
			return ;
		}

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
