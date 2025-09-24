/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:44:49 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/24 09:33:41 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executePart(Server &server, Client* c)
{
	if (isMember(c))
	{
		std::cout << "Debugging: Client " << c->getNick() << " is leaving channel " << _name << std::endl;
		removeMember(c);

		// The Client might be a members and Operator at the same time, so i need to check that as well.
		if (isOperator(c))
		{
			removeOperator(c);
			std::cout << "Debugging: Client " << c->getNick() << " was an operator and has been removed from operators list." << std::endl;
			notifyMembers(server, ":" + c->getNick() + " PART " + _name + " :Operator has left the channel");
		}
		else
			notifyMembers(server, ":" + c->getNick() + " PART " + _name);

		server.sendMsgToClient(c, ":" + c->getNick() + " PART " + _name + " :You have left the channel\r\n");
	}
	
	else
		c->sendNumericReply(server, 442, _name, "You're not on that channel");
}

void handlePart(Server &server, Client &client, const Command &cmd)
{
	std::cout << "debugging the Part CoMMANAD" << std::endl;
	if(cmd.params.empty())
	{
		//do same as join hnaya''''''''''''''''''''
		return;
	}
	std::string channelName = cmd.params[0];
	//hnaya also i need to check wach ila dar part without being in the channel wach khasni n3ti error ola la,
	Channel* channel = server.getChannel(channelName);
	if(!channel)
	{
		client.sendNumericReply(server, 403, channelName, "No such channel");
		return;
	}
	channel->executePart(server ,&client);
}