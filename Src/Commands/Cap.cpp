/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:40:39 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/15 14:46:18 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void handleCap(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
		return ;

	const std::string &subcmd = cmd.params[0];

	if (subcmd == "LS")
	{
		// Client asks which capabilities the server supports
		std::string msg = ":ft_irc CAP * LS :\r\n";
		server.sendMsgToClient(&client, msg);
	}
	else if (subcmd == "REQ")
	{
		// Client requests capabilities we don't support
		std::string msg = ":ft_irc CAP * NAK :" + cmd.params.back() + "\r\n";
		server.sendMsgToClient(&client, msg);
	}
	else if (subcmd == "END")
	{
		// End of negotiation — nothing to do
		return ;
	}
}