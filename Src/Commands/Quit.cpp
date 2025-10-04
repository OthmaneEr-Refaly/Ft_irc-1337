/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:40:45 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 16:59:44 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void	handleQuit(Server &server, Client &client, const Command &cmd)
{
	std::string reason = "Quit";
	
	if (!cmd.params.empty())
		reason = cmd.params[0];

	server.disconnectClient(client.getFd(), reason);
}
