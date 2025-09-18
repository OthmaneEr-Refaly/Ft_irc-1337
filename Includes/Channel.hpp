#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel 
{
	private:
	// ======= Identity =======
		std::string             _name;
		std::string             _topic;

	// ======= Membership =======
		std::set<Client*>       _members;       // Pointers owned by Server
		std::set<Client*>       _operators;
		std::set<std::string>   _invited_nicks;

	// ======= Modes / Settings =======
		bool                    _mode_invite_only;   // +i
		bool                    _mode_topic_ops_only;// +t
		std::string             _key;                // +k
		int                     _limit;              // +l (0 or -1 = unlimited)

	public:
	// ======= Canonical form =======
					Channel();
					Channel(const Channel& other);
		Channel&	operator=(const Channel& other);
					~Channel();
					Channel(const std::string& name);

	// ======= Getters =======
		const std::string&              getName() const;
		const std::string&              getTopic() const;
		const std::set<Client*>&        getMembers() const;
		const std::set<Client*>&        getOperators() const;
		const std::set<std::string>&    getInvitedNicks() const;
		bool                            isInviteOnly() const;
		bool                            isTopicOpsOnly() const;
		const std::string&              getKey() const;
		int                             getLimit() const;

	// ======= Setters =======
		void setTopic(const std::string& topic);
		void setInviteOnly(bool value);
		void setTopicOpsOnly(bool value);
		void setKey(const std::string& key);
		void setLimit(int limit);

	// ======= Membership Management =======
		bool addMember(Client* c);   // Returns true if successfully added
		void removeMember(Client* c);
		bool isMember(Client* c) const;

	// ======= Operator Management =======
		bool addOperator(Client* c); // Returns true if successfully added
		void removeOperator(Client* c);
		bool isOperator(Client* c) const;

	// ===== Invitation Management =====
		bool addInvite(const std::string& nick); // Returns true if successfully added
		void removeInvite(const std::string& nick);
		bool isInvited(const std::string& nick) const;

	// ===== JOIN/LEAVE Helpers =====
		void executeJoin(Client* c, const std::string& key); // Add to members/operators/invites
		bool canJoin(Client* c, const std::string& key) const; // Check invite/key/limit
		void handleLeave(Client* c); // Remove from members/operators/invites
		void notifyMembers(const std::string& message);



		void executePart(Client* c);
		void executeTopic(Client* c, const std::string& topic);
		void executeMode(Client* c, const std::string& mode, const std::string& param);

};

#endif
