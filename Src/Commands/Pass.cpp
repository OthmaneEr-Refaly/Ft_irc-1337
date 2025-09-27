/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:46:59 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/26 10:35:56 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void	handlePass(Server &server, Client &client, const Command &cmd)
{
   // Step 1: Check parameters
    if (cmd.params.empty() && !client.isPassOk())
	{
        client.sendNumericReply(server, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
        return;
    }

    // Step 2: Check if already registered
    if (client.isPassOk() || client.isRegistered())
	{
        client.sendNumericReply(server, ERR_ALREADYREGISTRED, "PASS", "You are already registered");
        return;
    }

    // Step 3: Checking the password
	if (cmd.params[0] != server.getPassword())
	{
        client.sendNumericReply(server, ERR_PASSWDMISMATCH, "PASS", "Password incorrect");
		server.disconnectClient(client.getFd(), "Bad Password");
		return;			
	}
	
	else
		client.setPassOk(true);
	
	server.tryRegister(client);
}
