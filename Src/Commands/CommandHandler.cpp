/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:18 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/15 20:33:05 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

static std::map<std::string, CommandFunction> commandMap; // need to undestand deeply how this map static declaration will work, and why using it as a static.

void	initCommandMap()
{
	commandMap["PASS"] = handlePass;
	commandMap["NICK"] = handleNick;
	commandMap["USER"] = handleUser;
}

void	dispatchCommand(Server &server, Client &client, const Command &cmd)
{
	std::map<std::string, CommandFunction>::iterator it = commandMap.find(cmd.command);
	
	if (it != commandMap.end())
	{
		it->second(server, client, cmd);
	}
	else
	{
		client.sendNumericReply(ERR_UNKNOWNCOMMAND, cmd.command, "Unknown command");
	}
}
