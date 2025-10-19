/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:48:39 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/18 11:55:09 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void	handleNick(Server &server, Client &client, const Command &cmd)
{
    std::cout << "Debugging: Entered handleNick function" << std::endl;

    if (cmd.params.empty())
    {
        std::cout << "Debugging: No nickname provided in NICK command" << std::endl;
        client.sendNumericReply(server, ERR_NONICKNAMEGIVEN, "NICK", "No Nickname given");
        return;
    }
    
    std::string newNick = cmd.params[0];
    std::cout << "Debugging: Requested new nickname: " << newNick << std::endl;

    if (!isNickValid(newNick))
    {
        std::cout << "Debugging: Invalid nickname: " << newNick << std::endl;
        client.sendNumericReply(server, ERR_ERRONEUSNICKNAME, newNick, "Invalid Nickname");
        return;
    }
    
    std::string normNick = normalizeCase(newNick);
    std::cout << "Debugging: Normalized nickname: " << normNick << std::endl;

    if (server.isNicknameInUse(normNick))
    {
        std::cout << "Debugging: Nickname already in use: " << normNick << std::endl;
        client.sendNumericReply(server, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
        return;
    }

    if (!client.getNick().empty())
    {
        std::string oldNormNick = normalizeCase(client.getNick());
        std::cout << "Debugging: Unregistering old nickname: " << oldNormNick << std::endl;
        server.unregisterNickname(oldNormNick);
    }
    
    std::string oldNick = client.getNick();
    client.setNick(newNick);
    std::cout << "Debugging: Updated client nickname from " << oldNick << " to " << newNick << std::endl;

    server.registerNickname(normNick, &client);
    std::cout << "Debugging: Registered new nickname: " << normNick << std::endl;

    if (client.isRegistered())
    {
        std::cout << "Debugging: Client is already registered" << std::endl;

        std::string nickMsg = ":" + oldNick + "!" + client.getUser() + "@" + client.getHost()
                            + " NICK :" + newNick + "\r\n";
        std::cout << "Debugging: Sending NICK message to channel members: " << nickMsg << std::endl;

        const std::set<std::string>& channels = client.getChannels();
        for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        {
            Channel *chan = server.getChannel(*it);
            if (!chan)
            {
                std::cout << "Debugging: Channel not found: " << *it << std::endl;
                continue;
            }

            const std::set<Client*>& members = chan->getMembers();
            for (std::set<Client*>::const_iterator mit = members.begin(); mit != members.end(); ++mit)
            {
                if (*mit != &client)
                {
                    std::cout << "Debugging: Sending NICK message to member: " << (*mit)->getNick() << std::endl;
                    server.sendMsgToClient(*mit, nickMsg);
                }
            }
        }
    }
    else
    {
        std::cout << "Debugging: Client is not registered, attempting to register" << std::endl;
        server.tryRegister(client);
    }

    std::cout << "Debugging: Exiting handleNick function" << std::endl;
}
