/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:06:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/23 15:56:34 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void	handleUser(Server &server, Client &client, const Command &cmd)
{
	
	if (client.isRegistered())
	{
		client.sendNumericReply(ERR_ALREADYREGISTRED, "USER", "You may not reregister");
		return ;
	}
	
	if (cmd.params.size() < 4)
	{
		client.sendNumericReply(ERR_NEEDMOREPARAMS, "USER", "not enough parameters");
		return ;
	}
	
	client.setUser(cmd.params[0]);
	client.setRealname(cmd.params[3]);

	server.tryRegister(client);
}
