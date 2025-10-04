/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:44:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 18:57:38 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"


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
		client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
		return ;
	}
	
	std::string targetNick  = cmd.params[0];
	std::string channelName = cmd.params[1];

	Client *targetClient = server.findClientByNick(normalizeCase(targetNick));
	if (!targetClient)
	{
		client.sendNumericReply(server, ERR_NOSUCHNICK, targetNick, "No such nick");
		return ;
	}

	Channel *channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(server, ERR_NOSUCHCHANNEL, channelName, "No such channel");
		return ;
	}
	
	if (!channel->isMember(&client))
	{
		client.sendNumericReply(server, ERR_NOTONCHANNEL, channelName, "You're not on that channel");
		return ;
	}
	
	if (channel->isInviteOnly() && !channel->isOperator(&client))
	{
		client.sendNumericReply(server, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
		return ;
	}
	
	if (channel->isMember(targetClient))
	{
		client.sendNumericReply(server, ERR_USERONCHANNEL, targetNick + " " + channelName, "is already on channel");
		return ;
	}
	
	channel->addInvite(targetNick);
	std::string formattedMsg = inviteMsgFormat(client, channelName, targetNick);
	server.sendMsgToClient(targetClient, formattedMsg);
	client.sendNumericReply(server, RPL_INVITING, channelName + " " + targetNick, "");
}
