#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Client.hpp"
#include "Server.hpp"

class   Client;
struct  Command;

// ===== Command Dispatcher =====
typedef void	(*CommandFunction)(Server &, Client &, const Command &);
void			initCommandMap();
void			dispatchCommand(Server &server, Client &client, const Command &cmd);

// ===== Registration Commands =====
	void			handlePass(Server &server, Client &client, const Command &cmd);
	void			handleNick(Server &server, Client &client, const Command &cmd);
	void			handleUser(Server &server, Client &client, const Command &cmd);

// ===== Channel Management Commands =====
	void		 	handleJoin(Server &server, Client &client, const Command &cmd);
	void		 	handlePart(Server &server, Client &client, const Command &cmd);
	void		 	handleMode(Server &server, Client &client, const Command &cmd);
	void		 	handleTopic(Server &server, Client &client, const Command &cmd);
	void			handleKick(Server &server, Client &client, const Command &cmd);
	void			handleInvite(Server &server, Client &client, const Command &cmd);

// ===== Messaging Commands =====
	void			handlePrivmsg(Server &server, Client &client, const Command &cmd);
	void			handleNotice(Server &server, Client &client, const Command &cmd);

// ===== Connection Management =====
	void			handleQuit(Server &server, Client &client, const Command &cmd);


// ===== Helper Functions =====
bool						isNickValid(const std::string &nick);
std::string 				normalizeNick(const std::string &newNick);
std::vector<std::string>	splitTargets(const std::string targets);
bool isValidChannelName(const std::string& channelName);

#endif
