/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:56:26 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 16:56:30 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

// ======= Canonical form =======
    Channel::Channel() 
        : _mode_invite_only(false), _mode_topic_ops_only(false), _limit(0) {}

    Channel::Channel(const std::string& name)
	    : _name(name), _mode_invite_only(false), _mode_topic_ops_only(false), _limit(0) {}

    Channel::Channel(const Channel& other)
    {
        *this = other;
    }

    Channel& Channel::operator=(const Channel& other)
    {
        if (this != &other)
        {
            _name = other._name;
            _topic = other._topic;
            _members = other._members;
            _operators = other._operators;
            _invited_nicks = other._invited_nicks;
            _mode_invite_only = other._mode_invite_only;
            _mode_topic_ops_only = other._mode_topic_ops_only;
            _key = other._key;
            _limit = other._limit;
        }
        return *this;
    }

    Channel::~Channel() {}

// ===== Getters =====
    const std::string&              Channel::getName() const { return (_name); }
    const std::string&              Channel::getTopic() const { return (_topic); }
    const std::set<Client*>&        Channel::getMembers() const { return (_members); }
    const std::set<Client*>&        Channel::getOperators() const { return (_operators); }
    const std::set<std::string>&    Channel::getInvitedNicks() const { return (_invited_nicks); }
    bool                            Channel::isInviteOnly() const { return (_mode_invite_only); }
    bool                            Channel::isTopicOpsOnly() const { return (_mode_topic_ops_only); }
    const std::string&              Channel::getKey() const { return (_key); }
    int                             Channel::getLimit() const { return (_limit); }

// ======= Setters =======
    void Channel::setTopic(const std::string& topic) { _topic = topic; }
    void Channel::setInviteOnly(bool value) { _mode_invite_only = value; }
    void Channel::setTopicOpsOnly(bool value) { _mode_topic_ops_only = value; }
    void Channel::setKey(const std::string& key) { _key = key; }
    void Channel::setLimit(int limit) { _limit = limit; }
