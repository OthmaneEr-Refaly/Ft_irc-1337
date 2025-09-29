/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHelpers.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:50:40 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/29 10:15:28 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include "../../Includes/CommandHandler.hpp"
#include "../../Includes/NumericReplies.hpp"

std::vector<std::string> splitTargets(const std::string targets)
{
	std::vector<std::string> result;
	std::stringstream sstream(targets);
	std::string item;

	while (std::getline(sstream, item, ','))
	{
		if (!item.empty())
			result.push_back(item);
	}
	return result;
}

bool isNickValid(const std::string &nick)
{
	// Check NICK size (nickname RFC limit size is 9) 
	if (nick.size() > 9)
		return (false);
	// Check NICK first char (nickname first char should be alphabetic)
	if (!std::isalpha(nick[0]))
		return (false);
	// Checkin if NICK chars are all allowed by RFC rules
	for(size_t i = 0; i < nick.size(); i++)
	{
		char c = nick[i];

		if (!(std::isalnum(c) ||  c == '-' || c == '_' || c == '[' || c == ']' || 
        					c == '\\' || c == '`' || c == '^' || c == '{' ||
							c == '|' || c == '}'))
		{
			return (false);
		}
	}
	return (true);
}

std::string normalizeNick(const std::string &newNick)
{
	std::string norm = newNick;

	for(size_t i = 0; i < norm.size(); i++)
	{
		if (std::isalpha(norm[i]))
		{
			norm[i] = std::tolower(norm[i]);
		}
		else 
		{
			if (norm[i] == '{')
				norm[i] = '[';
			else if (norm[i] == '}')
				norm[i] = ']';
			else if (norm[i] == '|')
				norm[i] = '\\';
		}
	}
	return (norm);
}
