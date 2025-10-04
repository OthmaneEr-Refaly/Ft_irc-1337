/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:28:01 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 10:44:52 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

std::string kickMsgFormat(const Client& op, const std::string &channelName,
						  const std::string &targetNick, const std::string &reason)
{
	std::string msg = ":" + op.getNick() + "!"
					+ op.getUser() + "@" + op.getHost()
					+ " KICK " + channelName + " "
					+ targetNick + " :" + reason
					+"\r\n";
	return (msg); 	
}

void	handleKick(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.size() < 2)
	{
		client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
		return;
	}

	std::string channelName = cmd.params[0];
	std::string reason;
	if (cmd.params.size() > 2)
		reason = cmd.params[2];

	Channel *channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(server, ERR_NOSUCHCHANNEL, channelName, "No such channel");
		return;
	}

	if (!channel->isOperator(&client))
	{
		client.sendNumericReply(server, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
		return;
	}

	const std::vector<std::string> targets = splitTargets(cmd.params[1]);

	for (size_t i = 0; i < targets.size(); ++i)
	{
		std::string targetNick = targets[i];
		Client *targetClient = server.findClientByNick(normalizeNick(targetNick));

		if (!targetClient)
		{
			client.sendNumericReply(server, ERR_NOSUCHNICK, targetNick, "No such nick");
			continue;
		}

		if (!channel->isMember(targetClient))
		{
			client.sendNumericReply(server, ERR_USERNOTINCHANNEL, targetNick + " " + channelName,
									"They aren't in that channel");
			continue;
		}

		channel->removeMember(targetClient);
		channel->removeInvite(targetNick);
		targetClient->removeChannel(channelName);
		if (channel->isOperator(targetClient))
			channel->removeOperator(targetClient);

		std::string formattedMsg = kickMsgFormat(client, channelName, targetNick, reason);

		server.sendMsgToClient(targetClient, formattedMsg);

		const std::set<Client*> &members = channel->getMembers();
		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			server.sendMsgToClient(*it, formattedMsg);
		}

		if (channel->getMembers().empty())
		{
			server.removeChannel(channelName);
			break;
		}
	}
}
