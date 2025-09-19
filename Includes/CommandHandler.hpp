/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 11:30:19 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/19 18:25:20 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Client.hpp"
#include "Server.hpp"

class   Client;
struct  Command;

typedef void	(*CommandFunction)(Server &, Client &, const Command &);
void			initCommandMap();
void			dispatchCommand(Server &server, Client &client, const Command &cmd);
// registration commands:
	void	handlePass(Server &server, Client &client, const Command &cmd);
	void	handleNick(Server &server, Client &client, const Command &cmd);
	void	handleUser(Server &server, Client &client, const Command &cmd);
	void 	handleJoin(Server &server, Client &client, const Command &cmd);
	void 	handlePart(Server &server, Client &client, const Command &cmd);
	void 	handleMode(Server &server, Client &client, const Command &cmd);
	void 	handleTopic(Server &server, Client &client, const Command &cmd);
	void	handlePrivmsg(Server &server, Client &client, const Command &cmd);
	void	handleKick(Server &server, Client &client, const Command &cmd);
	void	handleInvite(Server &server, Client &client, const Command &cmd);
	
// helper functions:
	std::string normalizeNick(const std::string &newNick);

#endif
