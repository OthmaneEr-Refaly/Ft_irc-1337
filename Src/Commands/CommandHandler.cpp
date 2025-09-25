/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:18 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/25 11:57:30 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"
#include <sstream>


static std::map<std::string, CommandFunction> commandMap; // need to undestand deeply how this map static declaration will work, and why using it as a static.

void	initCommandMap()
{
	commandMap["PASS"]    = handlePass;
	commandMap["NICK"]    = handleNick;
	commandMap["USER"]    = handleUser;
	commandMap["PRIVMSG"] = handlePrivmsg;
	commandMap["KICK"]    = handleKick;
	commandMap["INVITE"]  = handleInvite;
	commandMap["JOIN"]    = handleJoin;
	commandMap["PART"]    = handlePart;
	commandMap["MODE"]    = handleMode;
	commandMap["TOPIC"]   = handleTopic;
	commandMap["QUIT"]    = handleQuit;
	commandMap["NOTICE"]  = handleNotice;
}

void	dispatchCommand(Server &server, Client &client, const Command &cmd)
{
	if (!client.isRegistered())
	{
		if (cmd.command == "PASS")
		{
			handlePass(server, client, cmd);
			return ;
		}
		
		if (cmd.command == "NICK")
		{
			handleNick(server, client, cmd);
			return ;
		}
		if (cmd.command == "USER")
		{
			handleUser(server, client, cmd);
			return ;
		}
		if (cmd.command == "QUIT")
		{
			handleQuit(server, client, cmd);
			return ;
		}
	}
	
	std::map<std::string, CommandFunction>::iterator it = commandMap.find(cmd.command);
	
	if (it != commandMap.end())
		it->second(server, client, cmd);
	else
		client.sendNumericReply(server, ERR_UNKNOWNCOMMAND, cmd.command, "Unknown command");
}

