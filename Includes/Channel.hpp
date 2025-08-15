#ifndef CHANNEL_HPP
#define CHENNEL_HPP

#include <string>

class Client;

class Channel
{
    private:
	// ======= Identity =======
    std::string				_name;
    std::string				_topic;

	// ======= Membership =======
	std::set<Clients*>		_members;				// Poiters owned by Server **SeeNotes(`1)**
	std::set<>Clients*>		_operators;
	std::set<std::string>	_invited_nicks;

	// ======= Modes / Settings =======
	bool					_mode_invite_only;		// +i
	bool					_mode_topic_ops_only;	// +t
	std::string				_key; 					// +k
	int						_limit;					// +l (0 or -1 = unlimited)

	public:
				Channel();
				Channel(const Channel& other);
	Channel& 	operator=(const Channel& other);
				~Channel();
				Channel(const std::string& name);

	// ======= Getters =======
	const std::string&				getName() const;
	const std::string&				getTopic() const;
    const std::set<Client*>&		getMembers() const;
    const std::set<Client*>&		getOperators() const;
    const std::set<std::string>&	getInvitedNicks() const;
    bool 							isInviteOnly() const;
    bool 							isTopicOpsOnly() const;
    const							std::string& getKey() const;
    int								getLimit() const;

	// ======= Setters =======
    void 							setTopic(const std::string& topic);
    void 							setInviteOnly(bool value);
    void 							setTopicOpsOnly(bool value);
    void							setKey(const std::string& key);
    void							setLimit(int limit);	

	// ======= Membership Management =======
	bool							addMember(Client* c); // Returns true if successfully added
    void							removeMember(Client* c);
    bool							isMember(Client* c) const;

	// ======= Operator Management =======
    bool							addOperator(Client* c); // Returns true if successfully added
    void							removeOperator(Client* c);
    bool							isOperator(Client* c) const;	

    // ===== Invitation management =====
    bool							addInvite(const std::string& nick); // Returns true if successfully added
    void							removeInvite(const std::string& nick);
    bool							isInvited(const std::string& nick) const;
};

#endif

/* ===== Notes ===== :
	
	`1 - the Server creates Client objects when new users connect.
		The Channel stores Client* pointers to those same clients,but does not create or delete them.
		When a user disconnects, Server deletes the Client object, and the Channel must just remove the pointer from its set.
		If Channel tried to delete them too → double free error 💥.

*/
