/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:44:49 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/06 09:34:26 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Channel::executePart(Server &server, Client* c)
{
    if (isMember(c))
    {
        std::cout << "Debugging: Client " << c->getNick() << " is leaving channel " << _name << std::endl;
        removeMember(c);

        // Notify other members
        std::string partMessage = formatMessage(
            c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
            "PART",
            _name,
            "Operator has left the channel"
        );

        if (isOperator(c))
        {
            removeOperator(c);
            std::cout << "Debugging: Client " << c->getNick() << " was an operator and has been removed from operators list." << std::endl;
            notifyMembers(server, partMessage);
        }
        else
        {
            partMessage = formatMessage(
                c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
                "PART",
                _name,
                ""
            );
            notifyMembers(server, partMessage);
        }

        // Send confirmation to the client
        std::string clientMessage = formatMessage(
            c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
            "PART",
            _name,
            "You have left the channel"
        );
        server.sendMsgToClient(c, clientMessage);
    }
    else
    {
        // Send error if the client is not in the channel
        c->sendNumericReply(server, 442, _name, "You're not on that channel");
    }
}

void handlePart(Server &server, Client &client, const Command &cmd)
{
    std::cout << "Debugging the PART command" << std::endl;

    if (cmd.params.empty())
    {
        std::cout << "Debugging: No parameters provided for PART" << std::endl;
        client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
        return;
    }

    std::string channelNames = cmd.params[0];
    const std::vector<std::string> targets = splitTargets(channelNames);

    for (size_t i = 0; i < targets.size(); ++i)
    {
        std::string channelName = normalizeChannelName(targets[i]);
        std::cout << "Debugging: Processing channel '" << channelName << "'" << std::endl;

        Channel* channel = server.getChannel(channelName);
        if (!channel)
        {
            // Send error if the channel does not exist
            client.sendNumericReply(server, ERR_NOSUCHCHANNEL, channelName, "No such channel");
            continue;
        }

        channel->executePart(server, &client);
    }
}

