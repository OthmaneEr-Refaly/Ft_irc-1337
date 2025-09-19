#include "../Includes/Channel.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/CommandHandler.hpp"
#include "../Includes/NumericReplies.hpp"
#include <stdlib.h>

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

    void Channel::executeJoin(Client* c ,const std::string& key) {

        //============ F HAD L FUNCTION BDAL DIK SEND MESSAGE BL FUNCTION DYALK NTA DYAL L ERRORS
        if (isMember(c)) {
            c->sendMessage("443 " + c->getNick() + " " + _name + " :is already on channel");
            return;
        }

        if (canJoin(c, key)) {
            addMember(c);
            if (_members.size() == 1) {
                addOperator(c);
            }
            removeInvite(c->getNick());

            notifyMembers(":" + c->getNick() + " JOIN " + _name);

            if (!_topic.empty()) {
                c->sendMessage(":" + _name + " TOPIC :" + _topic);
            }


            //error in test tesst JOIN #room1,#rrom2 "radi ykhasni nhadliha fl parsing ri nasaha for now".
            std::string memberList = ":";
            for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
                memberList += (*it)->getNick() + " ";
            }
            c->sendMessage(memberList);
        } else {
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

    void Channel::notifyMembers(const std::string& message)
	{
        for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
		{
            (*it)->sendMessage(message);
        }
    }

    void Channel::executePart(Client* c) {
        if (isMember(c)) {
            std::cout << "Debugging: Client " << c->getNick() << " is leaving channel " << _name << std::endl;
            removeMember(c);

            // The Client might be a members and Operator at the same time, so i need to check that as well.
            if (isOperator(c)) {
                removeOperator(c);
                std::cout << "Debugging: Client " << c->getNick() << " was an operator and has been removed from operators list." << std::endl;
                notifyMembers(":" + c->getNick() + " PART " + _name + " :Operator has left the channel");
            } else {
                notifyMembers(":" + c->getNick() + " PART " + _name);
            }

            c->sendMessage(":" + c->getNick() + " PART " + _name + " :You have left the channel");
        } else {
            c->sendMessage("442 " + c->getNick() + " " + _name + " :You're not on that channel");
        }
    }

    void Channel::executeTopic(Client* c, const std::string& topic) {
        std::cout << "Debugging: executeTopic called by client " << c->getNick() << " on channel " << _name << std::endl;

        if (!isMember(c)) {
            std::cout << "Debugging: Client " << c->getNick() << " is not a member of channel " << _name << std::endl;
            c->sendMessage("442 " + c->getNick() + " " + _name + " :You're not on that channel");
            return;
        }

        if (topic.empty()) {
            if (!_topic.empty()) {
                std::cout << "Debugging: Sending current topic to client " << c->getNick() << std::endl;
                c->sendMessage("332 " + c->getNick() + " " + _name + " :" + _topic);
            } else {
                std::cout << "Debugging: No topic is set for channel " << _name << std::endl;
                c->sendMessage("331 " + c->getNick() + " " + _name + " :No topic is set");
            }
            return;
        }

        if (_mode_topic_ops_only && !isOperator(c)) {
            std::cout << "Debugging: Client " << c->getNick() << " is not an operator and cannot set the topic" << std::endl;
            c->sendMessage("482 " + c->getNick() + " " + _name + " :You're not a channel operator");
            return;
        }

        std::cout << "Debugging: Setting new topic for channel " << _name << " to: " << topic << std::endl;
        setTopic(topic);

        std::cout << "Debugging: Notifying members about the new topic" << std::endl;
        notifyMembers(":" + c->getNick() + " TOPIC " + _name + " :" + topic);
    }

    void Channel::executeMode(Client* c, const std::string& mode, const std::string& param) {
        std::cout << "Debugging: executeMode called by client " << c->getNick() << " on channel " << _name << std::endl;

        if (!isOperator(c)) {
            std::cout << "Debugging: Client " << c->getNick() << " is not an operator and cannot change modes" << std::endl;
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

            std::cout << "Debugging: Processing mode flag '" << modeChar << "' with addMode=" << addMode << std::endl;
            ///KASHNI NCHECKI WACH SWITCH KAYAN.
            switch (modeChar) {
                case 'i':
                    _mode_invite_only = addMode;
                    std::cout << "Debugging: Invite-only mode set to " << _mode_invite_only << std::endl;
                    break;

                case 't':
                    _mode_topic_ops_only = addMode;
                    std::cout << "Debugging: Topic-ops-only mode set to " << _mode_topic_ops_only << std::endl;
                    break;

                case 'k':
                    if (addMode) {
                        _key = param;
                        std::cout << "Debugging: Key set to '" << _key << "'" << std::endl;
                    } else {
                        _key.clear();
                        std::cout << "Debugging: Key cleared" << std::endl;
                    }
                    break;

                case 'l':
                    if (addMode) {
                        _limit = std::atoi(param.c_str());
                        std::cout << "Debugging: Limit set to " << _limit << std::endl;
                    } else {
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






    void handleJoin(Server &server, Client &client, const Command &cmd) {
        std::cout << " Debaging Processing JOIN command" << std::endl;

        if (cmd.params.empty()) {
            std::cout << " Debaging No parameters provided for JOIN" << std::endl;
            client.sendNumericReply(ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
            return;
        }

        std::string channelName = cmd.params[0];
        std::string key = cmd.params[1]; //mazal khasni nchiki if the key if optional wla la

        std::cout << " Debaging Channel name: " << channelName << ", Key: " << key << std::endl;

        Channel* channel = server.getChannel(channelName);
        if (!channel) {
            std::cout << " Debaging Channel does not exist, creating new channel" << std::endl;
            channel = server.createChannel(channelName);
        }

        channel->executeJoin(&client, key);
    }


    void handlePart(Server &server, Client &client, const Command &cmd){
        std::cout << "debugging the Part CoMMANAD" << std::endl;
        if(cmd.params.empty()){
            //do same as join hnaya''''''''''''''''''''
            return;
        }
        std::string channelName = cmd.params[0];
        //hnaya also i need to check wach ila dar part without being in the channel wach khasni n3ti error ola la,
        Channel* channel = server.getChannel(channelName);
        if(!channel){
            client.sendNumericReply(403, channelName, "No such channel");
            return;
        }
        channel->executePart(&client);
    }

    void handleTopic(Server &server, Client &client, const Command &cmd) {
        if (cmd.params.empty()) {
            client.sendNumericReply(ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
            return;
        }

        std::string channelName = cmd.params[0];
        std::string topic = (cmd.params.size() > 1) ? cmd.params[1] : "";

        Channel* channel = server.getChannel(channelName);
        if (!channel) {
            client.sendNumericReply(403, channelName, "No such channel");
            return;
        }

        channel->executeTopic(&client, topic);
    }


    void handleMode(Server &server, Client &client, const Command &cmd) {
        if(cmd.params.size() < 2) {
            client.sendNumericReply(ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
            return;
        }

        std::string channelName = cmd.params[0];
        std::string mode = cmd.params[1];
        std::string param = (cmd.params.size() > 2) ? cmd.params[2] : "";

        Channel* channel = server.getChannel(channelName);
        if (!channel) {
            client.sendNumericReply(403, channelName, "No such channel");
            return;
        }
        channel->executeMode(&client, mode, param);
    }