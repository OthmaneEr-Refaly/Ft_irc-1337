/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:48:39 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/29 10:06:51 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

void	handleNick(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(server, ERR_NONICKNAMEGIVEN, "NICK", "No Nickname given");
		return;
	}
	
	std::string	newNick = cmd.params[0];
	
	if (!isNickValid(newNick))
	{
		client.sendNumericReply(server, ERR_ERRONEUSNICKNAME, newNick, "Invalid Nickname");
		return ;
	}
	
	std::string normNick = normalizeNick(newNick);

	if (server.isNicknameInUse(normNick))
	{
		client.sendNumericReply(server, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
    	return;
	}

	if (!client.getNick().empty())
	{
		std::string oldNormNick = normalizeNick(client.getNick());
		server.unregisterNickname(oldNormNick);
	}
	
	client.setNick(newNick);
	server.registerNickname(normNick, &client);

	std::cout << normNick << std::endl;
	
	server.tryRegister(client);	
}
