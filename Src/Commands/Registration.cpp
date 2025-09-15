/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/15 12:58:33 by mobouifr         ###   ########.fr       */
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