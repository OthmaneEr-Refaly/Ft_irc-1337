/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:40:52 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/22 15:34:18 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"
#include "../../Includes/Bot.hpp"
#include <string>
#include <algorithm>

bool Channel::canJoin(Client* c, const std::string& key) const
{
	if(_mode_invite_only && !isInvited(c->getNick()))
		return false;
	std::cout << "the key is " << key << std::endl;
	std::cout << "they _key is " << _key << std::endl;
	if (!_key.empty() && _key != key)
		return false;
	if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
		return false;
	return true;
}

std::string normalizeChannelName(const std::string& channelName) {
    std::string normalized = channelName;

    for (size_t i = 0; i < normalized.size(); ++i) {
        if (normalized[i] >= 'A' && normalized[i] <= 'Z') {
            normalized[i] = normalized[i] - ('A' - 'a');
        }
        else if (normalized[i] == '{') {
            normalized[i] = '[';
        } else if (normalized[i] == '}') {
            normalized[i] = ']';
        } else if (normalized[i] == '|') {
            normalized[i] = '\\';
        } else if (normalized[i] == '~') {
            normalized[i] = '^';
        }
    }
    return normalized;
}

void Channel::executeJoin(Server &server, Client* c, const std::string& key)
{
    std::cout << "Debugging: Executing JOIN for client " << c->getNick() << " on channel " << _name << std::endl;

    if (isMember(c))
    {
        std::cout << "Debugging: Client is already a member of the channel" << std::endl;
        c->sendNumericReply(server, 443, _name, "is already on channel");
        return;
    }

    if (canJoin(c, key))
    {
        addMember(c);
        c->addChannel(_name);
        if (_members.size() == 1)
        {
            addOperator(c);
        }

        removeInvite(c->getNick());

        std::string joinMessage = formatMessage(
            c->getNick() + "!" + c->getUser() + "@" + c->getHost(),
            "JOIN",
            _name,
            ""
        );
        std::cout << "Debugging: Sending JOIN message: " << joinMessage << std::endl;
        notifyMembers(server, joinMessage);

        if (!_topic.empty()) {
            std::string topicMessage = formatMessage(
                "ircserv",
                "332",
                c->getNick() + " " + _name,
                _topic
            );
            std::cout << "Debugging: Sending topic message: " << topicMessage << std::endl;
            server.sendMsgToClient(c, topicMessage);
        } else {
            std::string noTopicMessage = formatMessage(
                "ircserv",
                "331",
                c->getNick() + " " + _name,
                "No topic is set"
            );
            std::cout << "Debugging: Sending no topic message: " << noTopicMessage << std::endl;
            server.sendMsgToClient(c, noTopicMessage);
        }

        std::string memberList;
        for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
            if (isOperator(*it)) {
                memberList += "@" + (*it)->getNick() + " ";
            } else {
                memberList += (*it)->getNick() + " ";
            }
        }

        std::string namesMessage = formatMessage(
            "ircserv",
            "353",
            c->getNick() + " = " + _name,
            memberList
        );
        std::cout << "Debugging: Sending NAMES message: " << namesMessage << std::endl;
        server.sendMsgToClient(c, namesMessage);

        std::string endOfNamesMessage = formatMessage(
            "ircserv",
            "366",
            c->getNick() + " " + _name,
            "End of /NAMES list"
        );
        std::cout << "Debugging: Sending end of NAMES message: " << endOfNamesMessage << std::endl;
        server.sendMsgToClient(c, endOfNamesMessage);
    }
    else
    {
        if (_mode_invite_only && !isInvited(c->getNick()))
            c->sendNumericReply(server, 473, _name, "Cannot join channel (+i)");
        else if (!_key.empty() && _key != key)
            c->sendNumericReply(server, 475, _name, "Cannot join channel (+k)");
        else if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
            c->sendNumericReply(server, 471, _name, "Cannot join channel (+l)");
    }
}

void handleJoin(Server &server, Client &client, const Command &cmd) {
    std::cout << "Debugging: Processing JOIN command" << std::endl;

    if (cmd.params.empty()) {
        std::cout << "Debugging: No parameters provided for JOIN" << std::endl;
        client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
        return;
    }

    std::string channelNames = cmd.params[0];
    std::string keys = (cmd.params.size() > 1) ? cmd.params[1] : "";

    const std::vector<std::string> channels = splitTargets(channelNames);
    const std::vector<std::string> keyList = splitTargets(keys);

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string channelName = normalizeChannelName(channels[i]);
        std::string key = (i < keyList.size()) ? keyList[i] : "";

        std::cout << "Debugging: Processing channel '" << channelName << "' with key '" << key << "'" << std::endl;

        if (!isValidChannelName(channelName)) {
            std::cout << "Debugging: Invalid channel name '" << channelName << "'" << std::endl;
            client.sendNumericReply(server, ERR_NOSUCHCHANNEL, channelName, "No such channel");
            continue;
        }

        // Validate the key (optional: add your own key validation logic)
        if (!key.empty() && key[0] == '+') {
            std::cout << "Debugging: Invalid key '" << key << "' (looks like a mode)" << std::endl;
            client.sendNumericReply(server, ERR_BADCHANNELKEY, channelName, "Invalid key");
            continue;
        }

        Channel* channel = server.getChannel(channelName);
        if (!channel) 
        {
            std::cout << "Debugging: Channel does not exist, creating new channel" << std::endl;
            channel = server.createChannel(channelName);
            if (!key.empty()) {
                channel->setKey(key); // Only set the key if it's valid
            }
        }

        channel->executeJoin(server, &client, key);
        if (server.getBot() && !channel->isMember(server.getBot()))
        {
            channel->executeJoin(server, server.getBot(), "");
        }
    }
}

bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty())
        return false;

    // Must start with '#' or '&'
    if (channelName[0] != '#' && channelName[0] != '&')
        return false;

    // Must have at least one character after '#' or '&'
    if (channelName.size() == 1)
        return false;

    // Must not exceed 200 characters
    if (channelName.size() > 200)
        return false;

    // Must not contain spaces, commas, or control characters
    for (size_t i = 0; i < channelName.size(); ++i) {
        if (channelName[i] == ' ' || channelName[i] == ',' || iscntrl(channelName[i]))
            return false;
    }

    return true;
}


