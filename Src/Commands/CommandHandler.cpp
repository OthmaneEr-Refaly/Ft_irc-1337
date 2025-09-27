/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:18 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/26 11:04:57 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

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
	if (cmd.command == "QUIT")
    {
        handleQuit(server, client, cmd);
        return;
    }
    
	if (!client.isPassOk())
    {
        if (cmd.command == "PASS")
            handlePass(server, client, cmd);
        else
            client.sendNumericReply(server, ERR_NOTREGISTERED, cmd.command, "Password required");

        return;
    }

    if (!client.isRegistered())
    {
        if (cmd.command == "PASS")
		{
        	client.sendNumericReply(server, ERR_ALREADYREGISTRED, cmd.command, "You may not reregister");
			return ;
		}
		if (cmd.command == "NICK")
        {
            handleNick(server, client, cmd);
            return;
        }
        if (cmd.command == "USER")
        {
            handleUser(server, client, cmd);
            return;
        }
        client.sendNumericReply(server, ERR_NOTREGISTERED, cmd.command, "You have not registered");
        return;
    }

	std::map<std::string, CommandFunction>::iterator it = commandMap.find(cmd.command);
	
	if (it != commandMap.end())
		it->second(server, client, cmd);
	else
		client.sendNumericReply(server, ERR_UNKNOWNCOMMAND, cmd.command, "Unknown command");
}
