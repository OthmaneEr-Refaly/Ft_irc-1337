/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:21:56 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/07 10:42:31 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"
#include "../../Includes/Bot.hpp"


std::string privmsgFormat(Client &client, const std::string &target, const std::string &msg)
{
	std::string fullMsg = ":" + client.getNick() + "!" 
						+ client.getUser() + "@" + client.getHost()
						+ " PRIVMSG " + target + " :" + msg + "\r\n";

	return (fullMsg);
}

void	handlePrivmsg(Server &server, Client &client, const Command &cmd) // the case of multipe target should be also set later
{
	if (cmd.params.empty())
	{
		client.sendNumericReply(server, ERR_NORECIPIENT, "PRIVMSG", "No recipient given (PRIVMSG)");
		return ;
	}

	if (cmd.params.size() < 2 || (cmd.params.size() >= 2 && cmd.params[1].empty()))
	{
		client.sendNumericReply(server, ERR_NOTEXTTOSEND, cmd.params[0], "No text to send");
		return ;
	}

	const std::vector<std::string> targets = splitTargets(cmd.params[0]);
	std::string msg = cmd.params[1];
	for (size_t i = 0; i < targets.size(); ++i)
	{
		const std::string &target = targets[i];
		std::string formattedMsg = privmsgFormat(client, target, msg);
		if (target[0] == '#' || target[0] == '&')
		{
			Channel *chan = server.getChannel(target);
			if (!chan)
			{
				client.sendNumericReply(server, ERR_NOSUCHCHANNEL, target, "No Such channel");
				continue ;
			}

			if (!chan->isMember(&client))
			{
				client.sendNumericReply(server, ERR_NOTONCHANNEL, target, "You're not on that channel");
				continue ;
			}

			const std::set<Client*> &members = chan->getMembers();
			for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
			{
				if (*it != &client) // this condition is to not send the msg to the sender
				{
					server.sendMsgToClient(*it, formattedMsg);
				}
			}
			Bot *bot = server.getBot();
			if (bot && chan->isMember(bot))
			{
				bot->respondToMessage(server, client,msg, target);
			}
		}

		else
		{
			std::string normNick = normalizeCase(target);
			 if (normNick == "bot")
            {
                Bot *bot = server.getBot();
                if (bot)
                {
                    bot->respondToMessage(server, client, msg, target);
                }
                continue;
            }
			if (!server.isNicknameInUse(normNick))
			{
				client.sendNumericReply(server, ERR_NOSUCHNICK, target, "No such nick/channel");
				continue ;
			}
			std::map<std::string, Client*>::const_iterator it = server.getNickToClient().find(normNick);
			if (it != server.getNickToClient().end())
			{
				Client *targetClient = it->second;
				server.sendMsgToClient(targetClient, formattedMsg);
			}
		}
	}
}
