/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:40:52 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/24 09:28:35 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

bool Channel::canJoin(Client* c, const std::string& key) const
{
	if(_mode_invite_only && !isInvited(c->getNick()))
		return false;
	// if (_mode_invite_only && !(isInvited(getName())))
	//     return false;
	if (_key != key)
		return false;
	if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
		return false;
	return true;
}

void Channel::executeJoin(Server &server, Client* c ,const std::string& key)
{
	//============ F HAD L FUNCTION BDAL DIK SEND MESSAGE BL FUNCTION DYALK NTA DYAL L ERRORS
	if (isMember(c))
	{
		c->sendNumericReply(server, 443, _name, "is already on channel");
		return;
	}

	if (canJoin(c, key))
	{
		addMember(c);
		if (_members.size() == 1)
			addOperator(c);
		
		removeInvite(c->getNick());

		notifyMembers(server, ":" + c->getNick() + " JOIN " + _name);

		if (!_topic.empty())
			server.sendMsgToClient(c, ":" + _name + " TOPIC :" + _topic + "\r\n");

		//error in test tesst JOIN #room1,#rrom2 "radi ykhasni nhadliha fl parsing ri nasaha for now".
		std::string memberList = ":";
		for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
			memberList += (*it)->getNick() + " ";

		server.sendMsgToClient(c, memberList + "\r\n");
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

void handleJoin(Server &server, Client &client, const Command &cmd)
{
	std::cout << " Debuging Processing JOIN command" << std::endl;

	if (cmd.params.empty())
	{
		std::cout << " Debuging No parameters provided for JOIN" << std::endl;
		client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
		return;
	}

	std::string channelName = cmd.params[0];
	std::string key = cmd.params[1]; //i still need to check if the key if optional wla la

	std::cout << " Debuging Channel name: " << channelName << ", Key: " << key << std::endl;

	Channel* channel = server.getChannel(channelName);
	if (!channel)
	{
		std::cout << " Debuging Channel does not exist, creating new channel" << std::endl;
		channel = server.createChannel(channelName);
	}

	channel->executeJoin(server, &client, key);
}
