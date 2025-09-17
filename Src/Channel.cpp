#include "../Includes/Channel.hpp"
#include "../Includes/Client.hpp"

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

    void Channel::handleJoin(Client* c, const std::string& key) {
        if (isMember(c)) {
            c->sendMessage("443 " + c->getNick() + " " + _name + " :is already on channel");
            return;
        }

        if (canJoin(c, key)) {
            addMember(c);
            if (_members.size() == 1) {
                addOperator(c); // First member becomes operator
            }
            removeInvite(c->getNick()); // Remove from invites if present

            // Notify all members of the new join
            notifyMembers(":" + c->getNick() + " JOIN " + _name);

            // Send the topic to the joining client
            if (!_topic.empty()) {
                c->sendMessage(":" + _name + " TOPIC :" + _topic);
            }

            // Send the list of current members to the joining client
            std::string memberList = ":";
            for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
                memberList += (*it)->getNick() + " ";
            }
            c->sendMessage(memberList);
        } else {
            // Handle join failure
            if (_mode_invite_only && !isInvited(c->getNick())) {
                c->sendMessage("473 " + c->getNick() + " " + _name + " :Cannot join channel (+i)");
            } else if (!_key.empty() && _key != key) {
                c->sendMessage("475 " + c->getNick() + " " + _name + " :Cannot join channel (+k)");
            } else if (_limit > 0 && static_cast<int>(_members.size()) >= _limit) {
                c->sendMessage("471 " + c->getNick() + " " + _name + " :Cannot join channel (+l)");
            }
        }
    }

    bool Channel::canJoin(Client* c, const std::string& key) const{
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

    void Channel::notifyMembers(const std::string& message) {
        for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
            (*it)->sendMessage(message);
        }
    }

    void Channel::handlePart(Client* c) {
        if (isMember(c)) {
            removeMember(c);

            // The Client might be a members and Operator at the same time, so i need to check that as well.
            if (isOperator(c)) {
                removeOperator(c);
                notifyMembers(":" + c->getNick() + " PART " + _name + " :Operator has left the channel");
            } else {
                notifyMembers(":" + c->getNick() + " PART " + _name);
            }

            c->sendMessage(":" + c->getNick() + " PART " + _name + " :You have left the channel");
        } else {
            c->sendMessage("442 " + c->getNick() + " " + _name + " :You're not on that channel");
        }
    }

    void Channel::handleTopic(Client* c, const std::string& topic) {
        if (!isMember(c)) {
            c->sendMessage("442 " + c->getNick() + " " + _name + " :You're not on that channel");
            return;
        }

        if (topic.empty()) {
            if (!_topic.empty()) {
                c->sendMessage("332 " + c->getNick() + " " + _name + " :" + _topic);
            } else {
                c->sendMessage("331 " + c->getNick() + " " + _name + " :No topic is set");
            }
            return;
        }

        if (_mode_topic_ops_only && !isOperator(c)) {
            c->sendMessage("482 " + c->getNick() + " " + _name + " :You're not a channel operator");
            return;
        }

        setTopic(topic);

        notifyMembers(":" + c->getNick() + " TOPIC " + _name + " :" + topic);
    }

    void Channel::handleMode(Client* c, const std::string& mode, const std::string& param) {
        if (!isOperator(c)) {
            c->sendMessage("482 " + c->getNick() + " " + _name + " :You're not a channel operator");
            return;
        }

        bool addMode = true;
        for (size_t i = 0; i < mode.size(); ++i) {
            char modeChar = mode[i];

            if (modeChar == '+') {
                addMode = true;
                continue;
            } else if (modeChar == '-') {
                addMode = false;
                continue;
            }

            switch (modeChar) {
                case 'i':
                    _mode_invite_only = addMode;
                    break;

                case 't':
                    _mode_topic_ops_only = addMode;
                    break;

                case 'k':
                    if (addMode) {
                        _key = param;
                    } else {
                        _key.clear();
                    }
                    break;

                case 'l':
                    if (addMode) {
                        _limit = std::atoi(param.c_str());
                    } else {
                        _limit = 0; 
                    }
                    break;

                default:
                    c->sendMessage("501 " + c->getNick() + " " + _name + " :Unknown mode flag");
                    break;
            }
        }

        notifyMembers(":" + c->getNick() + " MODE " + _name + " " + mode + " " + param);
    }
