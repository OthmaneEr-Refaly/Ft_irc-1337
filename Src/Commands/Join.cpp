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
	std::cout << "the key is " << key << std::endl;
	std::cout << "they _key is " << _key << std::endl;
	if (!_key.empty() && _key != key)
		return false;
	if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
		return false;
	return true;
}

void Channel::executeJoin(Server &server, Client* c ,const std::string& key)
{
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

	//hna i check if all is valid "meaning the channel name starting with & or # and the channel name is not more then 200 charachter"
	if(cmd.params[0][0] != '#' && cmd.params[0][0] != '&')
	{
		std::cout << " Debuging Invalid channel name provided for JOIN" << std::endl;
		client.sendNumericReply(server, ERR_NOSUCHCHANNEL, cmd.params[0], "No such channel");
		return;
	}

	if (cmd.params[0].size() > 200)
	{
		std::cout << " Debuging Channel name too long provided for JOIN" << std::endl;
		client.sendNumericReply(server, ERR_NOSUCHCHANNEL, cmd.params[0], "No such channel");
		return;
	}


	std::string channelName = cmd.params[0];
	std::string key;
	bool isMultipleChannels = false;
	if (channelName.find(',') != std::string::npos)
		isMultipleChannels = true;

	if(cmd.params.size() > 1)
	{
		std::cout << " Debuging No key provided for JOIN, using empty string as default" << std::endl;
		key = cmd.params[1];
	}
	else
		key = "";
	
	if (isMultipleChannels){
		const std::vector<std::string> targets = splitTargets(cmd.params[0]);
		for (size_t i = 0; i < targets.size(); ++i)
		{
			const std::string &channels = targets[i];
			std::cout << " Debuging channels are : " << channels << std::endl;
			Channel* channel = server.getChannel(channels);
			if (!channel)
			{
				std::cout << " Debuging Channel does not exist, creating new channel" << std::endl;
				channel = server.createChannel(channels);
			}
	
			channel->executeJoin(server, &client, key);
		}
	}
	else{
		Channel* channel = server.getChannel(channelName);
		if (!channel)
		{
			std::cout << " Debuging Channel does not exist, creating new channel" << std::endl;
			channel = server.createChannel(channelName);
		}
	
		channel->executeJoin(server, &client, key);
	}

}
