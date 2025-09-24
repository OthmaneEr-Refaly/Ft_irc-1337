/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelHelpers.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:55:12 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/24 09:23:06 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

// ======= Membership Management =======
    bool Channel::addMember(Client* c)
    {
        std::pair<std::set<Client*>::iterator, bool> result = _members.insert(c);
        return result.second; // true if actually inserted, false if already present
    }

    void Channel::removeMember(Client* c)
    {
        _members.erase(c);
    }

    bool Channel::isMember(Client* c) const
    {
        return _members.count(c) > 0;
    }

// ===== Operator Management =====
    bool Channel::addOperator(Client* c) 
    {
        std::pair<std::set<Client*>::iterator, bool> result = _operators.insert(c);
        return result.second;
    }

    void Channel::removeOperator(Client* c) 
    {
        _operators.erase(c);
    }

    bool Channel::isOperator(Client* c) const 
    {
        return _operators.count(c) > 0;
    }

// ===== Invitation Management =====
    bool Channel::addInvite(const std::string& nick)
    {
        std::pair<std::set<std::string>::iterator, bool> result = _invited_nicks.insert(nick);
        return result.second;
    }

    void Channel::removeInvite(const std::string& nick)
    {
        _invited_nicks.erase(nick);
    }

    bool Channel::isInvited(const std::string& nick) const
    {
        return _invited_nicks.count(nick) > 0;
    }

    void Channel::notifyMembers(Server &server, const std::string& message)
	{
        for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
		{
            server.sendMsgToClient(*it, message + "\r\n");
        }
    }
