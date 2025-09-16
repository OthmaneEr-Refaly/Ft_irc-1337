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

    void Channel::handleJoin(Client* c, const std::string& key){
        if (canJoin(c, key)) {
            addMember(c);
            if (_members.size() == 1) {
                addOperator(c); // First member becomes operator
            }
            removeInvite(c->getNick()); // Remove from invites if present
            // Notify channel members of the new join (need to be implemented here)
        }
        // else {
        //     // Handle join failure (e.g., send error message to client)
        // }
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