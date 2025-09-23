/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:50:43 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 16:53:30 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executeTopic(Client* c, const std::string& topic)
{
	std::cout << "Debugging: executeTopic called by client " << c->getNick() << " on channel " << _name << std::endl;

	if (!isMember(c))
	{
		std::cout << "Debugging: Client " << c->getNick() << " is not a member of channel " << _name << std::endl;
		c->sendNumericReply(ERR_NOTONCHANNEL, _name, "You're not on that channel");
		return;
	}

	if (topic.empty())
	{
		if (!_topic.empty())
		{
			std::cout << "Debugging: Sending current topic to client " << c->getNick() << std::endl;
			c->sendNumericReply(RPL_TOPIC, _name, _topic);
		} 
		else
		{
			std::cout << "Debugging: No topic is set for channel " << _name << std::endl;
			c->sendNumericReply(RPL_NOTOPIC, _name, "No topic is set");
		}
		return;
	}

	if (_mode_topic_ops_only && !isOperator(c))
	{
		std::cout << "Debugging: Client " << c->getNick() << " is not an operator and cannot set the topic" << std::endl;
		c->sendNumericReply(ERR_CHANOPRIVSNEEDED, _name, "You're not a channel operator");
		return;
	}

	std::cout << "Debugging: Setting new topic for channel " << _name << " to: " << topic << std::endl;
	setTopic(topic);

	std::cout << "Debugging: Notifying members about the new topic" << std::endl;
	notifyMembers(":" + c->getNick() + "!" + c->getUser() + "@" + c->getHost() +
				" TOPIC " + _name + " :" + topic);
}


void handleTopic(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
		return;
	}

	std::string channelName = cmd.params[0];
	std::string topic = (cmd.params.size() > 1) ? cmd.params[1] : "";

	Channel* channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(403, channelName, "No such channel");
		return;
	}

	channel->executeTopic(&client, topic);
}
