/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick_invite.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:44:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/20 11:32:07 by mobouifr         ###   ########.fr       */
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
		client.sendNumericReply(ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
		return ;
	}
	std::string channelName = cmd.params[0];
	std::string targetNick  = cmd.params[1];
	std::string reason;
	if (cmd.params.size() > 2)
		reason = cmd.params[2];
	
	Channel *channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(ERR_NOSUCHCHANNEL, channelName, "No such channel");
		return ;
	}
	if (!channel->isOperator(&client))
	{
		client.sendNumericReply(ERR_CHANOPRIVSNEEDED, "KICK", "You're not channel operator");
		return ;
	}
	
	Client *targetClient = server.findClientByNick(normalizeNick(targetNick));
	if (!targetClient)
	{
		client.sendNumericReply(ERR_NOSUCHNICK, targetNick, "No such nick");
		return ;
	}
	if (!channel->isMember(targetClient))
	{
		client.sendNumericReply(ERR_USERNOTINCHANNEL, targetNick + " " + channelName, "they aren't in that channel");
		return ;
	}
	
	channel->removeMember(targetClient);
	channel->removeInvite(targetNick);
	targetClient->removeChannel(channelName);
	if (channel->isOperator(targetClient))
		channel->removeOperator(targetClient);

	std::string formattedMsg = kickMsgFormat(client, channelName, targetNick, reason); 
	server.sendMsgToClient(targetClient, formattedMsg);	
		
	if (channel->getMembers().empty())
	{
		server.removeChannel(channelName);
		return ;
	}
	else
	{
		const std::set<Client*> &members = channel->getMembers();
		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			server.sendMsgToClient(*it, formattedMsg);
		}
	}
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