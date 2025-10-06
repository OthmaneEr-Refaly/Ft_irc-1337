/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:44:49 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/06 09:12:47 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executePart(Server &server, Client* c)
{
	//handle this case :
	//MODE #room1 +itkl secretKey 100
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
		client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
		return;
	}
	std::string channelName = cmd.params[0];
	bool isMultipleChannels = false;
	if (channelName.find(',') != std::string::npos)
		isMultipleChannels = true;

	if(isMultipleChannels)
	{
		
		const std::vector<std::string> targets = splitTargets(cmd.params[0]);
		for (size_t i = 0; i < targets.size(); ++i){

			const std::string &channels = targets[i];
			std::cout << " Debuging channels are : " << channels << std::endl;
			Channel* channel = server.getChannel(channels);
			if (!channel)
			{
				client.sendNumericReply(server, 403, channelName, "No such channel");
				return;
			}
	
			channel->executePart(server ,&client);
		}
	}
	else{
		Channel* channel = server.getChannel(channelName);
		if(!channel)
		{
			client.sendNumericReply(server, 403, channelName, "No such channel");
			return;
		}
		channel->executePart(server ,&client);
	}
}
