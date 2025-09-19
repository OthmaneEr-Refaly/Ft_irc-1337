/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:04:10 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/19 09:20:30 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// =======================
//  Reply Codes (2xx, 3xx, 4xx, 5xx)
// =======================

// ---- Registration Replies ----
#define RPL_WELCOME           001   // ":Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST          002   // ":Your host is <servername>, running version <ver>"
#define RPL_CREATED           003   // ":This server was created <date>"
#define RPL_MYINFO            004   // "<servername> <version> <available user modes> <available channel modes>"

// ---- Channel Replies ----
#define RPL_NOTOPIC           331   // "<channel> :No topic is set"
#define RPL_TOPIC             332   // "<channel> :<topic>"
#define RPL_NAMREPLY          353   // "= <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
#define RPL_ENDOFNAMES        366   // "<channel> :End of NAMES list"

// ---- WHO/WHOIS ----
#define RPL_ENDOFWHOIS        318   // "<nick> :End of WHOIS list"


// ---- Registration Errors ----
#define ERR_NOSUCHNICK        401   // "<nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL     403   // "<channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN  404   // "<channel name> :Cannot send to channel"
#define ERR_UNKNOWNCOMMAND    421   // "<command> :Unknown command"
#define ERR_NONICKNAMEGIVEN   431   // ":No nickname given"
#define ERR_ERRONEUSNICKNAME  432   // "<nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE     433   // "<nick> :Nickname is already in use"
#define ERR_USERNOTINCHANNEL  441   // "<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL      442   // "<channel> :You're not on that channel"
#define ERR_USERONCHANNEL     443   // "<user> <channel> :is already on channel"
#define ERR_NOTREGISTERED     451   // ":You have not registered"
#define ERR_NEEDMOREPARAMS    461   // "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED  462   // ":You may not reregister"
#define ERR_PASSWDMISMATCH    464   // ":Password incorrect"
#define ERR_CHANNELISFULL     471   // "<channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE       472   // "<char> :is unknown mode char to me"
#define ERR_INVITEONLYCHAN    473   // "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN    474   // "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY     475   // "<channel> :Cannot join channel (+k)"
#define ERR_NOPRIVILEGES      481   // ":Permission Denied- You're not an IRC operator"
#define ERR_CHANOPRIVSNEEDED  482   // "<channel> :You're not channel operator"
#define ERR_UMODEUNKNOWNFLAG  501   // ":Unknown MODE flag"
#define ERR_USERSDONTMATCH    502   // ":Cannot change mode for other users"
#define ERR_NORECIPIENT 	  411   // ":No recipient given (PRIVMSG)"
#define	ERR_NOTEXTTOSEND	  412   // ":No text to send"

#endif 