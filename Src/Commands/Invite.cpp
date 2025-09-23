/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:44:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 15:41:20 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

std::string inviteMsgFormat(const Client& op, const std::string &channelName,
						  const std::string &targetNick)
{
	std::string msg = ":" + op.getNick() + "!"
					+ op.getUser() + "@" + op.getHost()
					+ " INVITE " + channelName + " "
					+ targetNick +"\r\n";
	return (msg); 	
}

void	handleInvite(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.size() < 2)
	{
		client.sendNumericReply(ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
		return ;
	}
	
	std::string targetNick  = cmd.params[0];
	std::string channelName = cmd.params[1];

	Client *targetClient = server.findClientByNick(normalizeNick(targetNick));
	if (!targetClient)
	{
		client.sendNumericReply(ERR_NOSUCHNICK, targetNick, "No such nick");
		return ;
	}

	Channel *channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(ERR_NOSUCHCHANNEL, channelName, "No such channel");
		return ;
	}
	
	if (!channel->isMember(&client))
	{
		client.sendNumericReply(ERR_NOTONCHANNEL, channelName, "You're not on that channel");
		return ;
	}
	
	if (channel->isInviteOnly() && !channel->isOperator(&client))
	{
		client.sendNumericReply(ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
		return ;
	}
	
	if (channel->isMember(targetClient))
	{
		client.sendNumericReply(ERR_USERONCHANNEL, targetNick + " " + channelName, "is already on channel");
		return ;
	}
	
	channel->addInvite(targetNick);
	std::string formattedMsg = inviteMsgFormat(client, channelName, targetNick);
	server.sendMsgToClient(targetClient, formattedMsg);
	client.sendNumericReply(RPL_INVITING, channelName + " " + targetNick, "");
}
