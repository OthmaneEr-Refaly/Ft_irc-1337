/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:04:10 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/14 12:04:13 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// Errors
#define ERR_NEEDMOREPARAMS    461
#define ERR_ALREADYREGISTRED  462
#define ERR_PASSWDMISMATCH    464
#define ERR_UNKNOWNCOMMAND    421
#define ERR_NONICKNAMEGIVEN   431
#define ERR_ERRONEUSNICKNAME  432
#define ERR_NICKNAMEINUSE     433
#define ERR_NOTREGISTERED     451
#define ERR_USERSDONTMATCH    502

// Success / basic replies
#define RPL_WELCOME           001
#define RPL_YOURHOST          002
#define RPL_CREATED           003
#define RPL_MYINFO            004
#define RPL_ENDOFMOTD         376

#endif 