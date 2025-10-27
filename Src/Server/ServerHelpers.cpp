/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHelpers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:12:59 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 17:07:46 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"
#include "../../Includes/Bot.hpp"


// // ===== Client management =====
// void Server::disconnectClient(int fd, const std::string &reason)
// {
//     std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
//     if (it == _fd_to_client.end())
//         return;

//     Client *client = it->second;

//     if (client->isClosing())
//         return;

//     std::string prefix = ":" + client->getNick() + "!" + client->getUser() + "@" + client->getHost();
//     std::string quitMsg = prefix + " QUIT :" + (reason.empty() ? "Client disconnected" : reason) + "\r\n";

//     const std::set<std::string> &chans = client->getChannels();
//     for (std::set<std::string>::const_iterator chit = chans.begin(); chit != chans.end(); ++chit)
//     {
//         Channel* chan = getChannel(*chit);
//         if (!chan) continue;

//         const std::set<Client*> &members = chan->getMembers();
//         for (std::set<Client*>::const_iterator mit = members.begin(); mit != members.end(); ++mit)
//         {
//             Client* member = *mit;
//             if (member->getFd() == fd) // skip the quitting client
//                 continue;
//             sendMsgToClient(member, quitMsg);
//         }
//     }

//     client->markForClose();

//     std::cout << "marked Client Fd=" << fd << " for close ( reason: " << reason << " )" << std::endl;
// }

// ===== Client management =====
void Server::disconnectClient(int fd, const std::string &reason)
{
    std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
    if (it == _fd_to_client.end())
        return;

    Client *client = it->second;

    if (client->isClosing())
        return;

    // RFC 1459 QUIT message format
    std::string prefix = ":" + client->getNick() + "!" + client->getUser() + "@" + client->getHost();
    std::string quitMsg = prefix + " QUIT :" + (reason.empty() ? "Client disconnected" : reason) + "\r\n";

    // === Deduplication set ===
    std::set<Client*> recipients;

    // Collect all unique recipients from all channels
    const std::set<std::string> &chans = client->getChannels();
    for (std::set<std::string>::const_iterator chit = chans.begin(); chit != chans.end(); ++chit)
    {
        Channel* chan = getChannel(*chit);
        if (!chan) continue;

        const std::set<Client*> &members = chan->getMembers();
        recipients.insert(members.begin(), members.end());
    }

    // Broadcast QUIT to all unique recipients (except quitting client)
    for (std::set<Client*>::iterator rit = recipients.begin(); rit != recipients.end(); ++rit)
    {
        Client* member = *rit;
        if (member == client) // skip self
            continue;
        sendMsgToClient(member, quitMsg);
    }

    // Cleanup: mark client for closing
    client->markForClose();

    std::cout << "marked Client Fd=" << fd << " for close ( reason: " << reason << " )" << std::endl;
}


void	Server::tryRegister(Client &client)
{
	if (client.isRegistered())
		return ;

	if (!client.isPassOk())
		return ;
	if (client.getNick().empty())
		return ;
	if (client.getUser().empty() || client.getRealname().empty())
		return;

	client.setRegistered(true);

	client.sendNumericReply(*this, RPL_WELCOME, client.getNick(),
	"Welcome to the IRC network " + client.getNick() + "!" + client.getUser() + "@" + client.getHost());

	client.sendNumericReply(*this, RPL_YOURHOST, client.getNick(),
	"Your host is ft_irc, running version sma9ma9");

	client.sendNumericReply(*this, RPL_CREATED, client.getNick(),
	"This server was created " + getCreationDate());

	client.sendNumericReply(*this, RPL_MYINFO, client.getNick(),
	"ft_irc laFin o itkl"/*message:"Server name, version, supported user & channel modes*/);  
}

// ===== Nickname management =====
void	Server::registerNickname(const std::string &nick, Client *client)
{
	_nick_to_client[nick] = client;
}

void	Server::unregisterNickname(const std::string &nick)
{
	_nick_to_client.erase(nick);
}

bool	Server::isNicknameInUse(const std::string &nick) const
{
	return (_nick_to_client.count(nick) > 0);
}

Client*	Server::findClientByNick(const std::string &nick)
{
	for (std::map<std::string, Client*>::iterator it = _nick_to_client.begin(); it != _nick_to_client.end(); ++it)
	{
		if (it->second->getNick() == nick)
		{
			return (it->second);
		}
	}
	return (NULL);
}

// ===== Channel management =====
Channel* Server::getChannel(const std::string& channelName) 
{
	std::cout << "Looking for channel: " << channelName << std::endl;
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it != _channels.end()) {
		std::cout << "Channel found: " << channelName << std::endl;
		return it->second;
	}
	std::cout << "Channel not found: " << channelName << std::endl;
	return NULL;
}

Channel* Server::createChannel(const std::string& channelName)
{
	std::cout << "debug Creating new channel: " << channelName << std::endl;
	Channel* newChannel = new Channel(channelName);
	_channels[channelName] = newChannel;
	std::cout << "debab Channel created and added to map: " << channelName << std::endl;
	if (_bot)
    {
        newChannel->executeJoin(*this,_bot,"");
        std::string joinMsg = ":" + _bot->getNick() + "!bot@localhost JOIN " + normalizeChannelName(channelName) + "\r\n";
        newChannel->notifyMembers(*this, joinMsg);
        std::cout << "debug Bot joined channel: " << normalizeChannelName(channelName) << std::endl;
    }
	return newChannel;
}

void Server::removeChannel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
	{
		delete it->second;
		_channels.erase(it);
		std::cout << "[debug] channel removed : " << channelName << std::endl;
	}
}

// ===== Messaging functions =====
void	Server::sendMsgToClient(Client *client, const std::string &msg)
{
	std::string safeMsg = enforceMessageLength(msg);
	client->enqueueOutput(safeMsg);
	enableWriteInterest(client->getFd());
}

std::string Server::enforceMessageLength(const std::string &rawMessage)
{
	if (rawMessage.size() <= IRC_MAX_LINE)
		return (rawMessage);

	size_t payloadStart = rawMessage.find(" :");
	if (payloadStart != std::string::npos)
	{
		std::string headerPart = rawMessage.substr(0, payloadStart + 2); // +2 for the " :"
		std::string payloadPart = rawMessage.substr(payloadStart + 2);

		size_t headerPartSize = headerPart.size() + 2; // +2 this time is for the upcoming \r\n
		if (headerPartSize >=IRC_MAX_LINE)
			return (rawMessage.substr(0, IRC_MAX_CONTENT) + "\r\n");
		
		size_t maxPayloadLength = IRC_MAX_LINE - headerPartSize;
		if (payloadPart.size() > maxPayloadLength)
			payloadPart = payloadPart.substr(0, maxPayloadLength);
			
		return (headerPart + payloadPart + "\r\n");
	}

	return (rawMessage.substr(0, IRC_MAX_CONTENT) + "\r\n");
}
