/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:37:42 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 16:53:11 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executeMode(Client* c, const std::string& mode, const std::string& param)
{
	std::cout << "Debugging: executeMode called by client " << c->getNick() << " on channel " << _name << std::endl;

	if (!isOperator(c)) 
	{
		std::cout << "Debugging: Client " << c->getNick() << " is not an operator and cannot change modes" << std::endl;
		c->sendMessage("482 " + c->getNick() + " " + _name + " :You're not a channel operator");
		return;
	}

	bool addMode = true;
	for (size_t i = 0; i < mode.size(); ++i)
	{
		char modeChar = mode[i];

		if (modeChar == '+')
		{
			addMode = true;
			continue;
		}
		else if (modeChar == '-')
		{
			addMode = false;
			continue;
		}

		std::cout << "Debugging: Processing mode flag '" << modeChar << "' with addMode=" << addMode << std::endl;
		///KASHNI NCHECKI WACH SWITCH KAYAN.
		
		switch (modeChar)
		{
			case 'i':
				_mode_invite_only = addMode;
				std::cout << "Debugging: Invite-only mode set to " << _mode_invite_only << std::endl;
				break;

			case 't':
				_mode_topic_ops_only = addMode;
				std::cout << "Debugging: Topic-ops-only mode set to " << _mode_topic_ops_only << std::endl;
				break;

			case 'k':
				if (addMode)
				{
					_key = param;
					std::cout << "Debugging: Key set to '" << _key << "'" << std::endl;
				}
				else
				{
					_key.clear();
					std::cout << "Debugging: Key cleared" << std::endl;
				}
				break;

			case 'l':
				if (addMode)
				{
					_limit = std::atoi(param.c_str());
					std::cout << "Debugging: Limit set to " << _limit << std::endl;
				}
				else
				{
					_limit = 0;
					std::cout << "Debugging: Limit cleared" << std::endl;
				}
				break;

			default:
				std::cout << "Debugging: Unknown mode flag '" << modeChar << "'" << std::endl;
				c->sendMessage("501 " + c->getNick() + " " + _name + " :Unknown mode flag");
				break;
		}
	}

	std::cout << "Debugging: Notifying members about mode change" << std::endl;
	notifyMembers(":" + c->getNick() + " MODE " + _name + " " + mode + " " + param);
}

void handleMode(Server &server, Client &client, const Command &cmd)
{

	if(cmd.params.empty())
	{
		client.sendNumericReply(ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
		return;
	}

	std::string channelName = cmd.params[0];
	Channel* channel = server.getChannel(channelName);
	if (!channel)
	{
		client.sendNumericReply(403, channelName, "No such channel");
		return;
	}

	std::string mode = cmd.params[1];
	std::string param = (cmd.params.size() > 2) ? cmd.params[2] : "";

	channel->executeMode(&client, mode, param);
}