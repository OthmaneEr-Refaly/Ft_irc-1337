/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 11:30:19 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/14 12:22:53 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Client.hpp"
#include "Server.hpp"

class   Client;
struct  Command;

typedef void	(*CommandFunction)(Client &, const Command &);
void			dispatchCommand(Client &client, const Command &cmd);

// registration commands:
	void	handlePass(Client &c, const Command &cmd);
	void	handleNick(Client &c, const Command &cmd);
	void	handleUser(Client &c, const Command &cmd);
	

#endif
