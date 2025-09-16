/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/16 11:26:12 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

void	handlePass(Server &server, Client &client, const Command &cmd)
{
   // Step 1: Check parameters
    if (cmd.params.empty())
	{
        client.sendNumericReply(ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
        return;
    }

    // Step 2: Check if already registered
    if (client.isRegistered() == true)
	{
        client.sendNumericReply(ERR_ALREADYREGISTRED, "PASS", "You are already registered");
        return;
    }

    // Step 3: Checking the password
	if (cmd.params[0] != server.getPassword())
	{
        client.sendNumericReply(ERR_PASSWDMISMATCH, "PASS", "Password incorrect");
		server.disconnectClient(client.getFd(), "Bad Password");
		return;			
	}
	
	else
		client.setPassOk(true);
	
    // Step 4: Optional debug log
    std::cout << "Client " << client.getNick() << " set password: " << cmd.params[0] << std::endl;
}

bool isNickValid(const std::string &nick)
{
	// Check NICK size (nickname RFC limit size is 9) 
	if (nick.size() > 9)
		return (false);
	// Check NICK first char (nickname first char should be alphabetic)
	if (!isalpha(nick[0]))
		return (false);
	// Checkin if NICK chars are all allowed by RFC rules
	for(size_t i = 0; i < nick.size(); i++)
	{
		char c = nick[i];

		if (!(isalnum(c) ||  c == '-' || c == '_' || c == '[' || c == ']' || 
        					c == '\\' || c == '`' || c == '^' || c == '{' ||
							c == '|' || c == '}'))
		{
			return (false);
		}
	}
	return (true);
}

std::string normalizeNick(const std::string &newNick)
{
	(void)newNick;
}

void	handleNick(Server &server, Client &client, const Command &cmd)
{
	std::string	newNick = cmd.params[0];
	
	if (cmd.params.empty())
	{
		client.sendNumericReply(ERR_NONICKNAMEGIVEN, "NICK", "No Nickname given");
		return;
	}
	
	if (!isNickValid(newNick))
	{
		client.sendNumericReply(ERR_ERRONEUSNICKNAME, newNick, "Invalid Nickname");
		return ;
	}
	
	std::string normNick = normalizeNick(newNick); //normalizeNick should be implemented

	
}

void	handleUser(Server &server, Client &client, const Command &cmd)
{
	(void)server;
	(void)client;
	(void)cmd;
}
