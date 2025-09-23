/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:18 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/20 18:33:42 by mobouifr         ###   ########.fr       */
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
	std::map<std::string, CommandFunction>::iterator it = commandMap.find(cmd.command);
	std::cout << "SEARCHING THE COMMANDS =============" << std::endl;
	
	if (it != commandMap.end())
	{
		it->second(server, client, cmd);
	}
	else
	{
		client.sendNumericReply(ERR_UNKNOWNCOMMAND, cmd.command, "Unknown command");
	}
}

// ==== Helper function ====
std::vector<std::string> splitTargets(const std::string targets)
{
	std::vector<std::string> result;
	std::stringstream sstream(targets);
	std::string item;

	while (std::getline(sstream, item, ','))
	{
		if (!item.empty())
			result.push_back(item);
	}
	return result;
}
