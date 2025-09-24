/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHelpers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:59:05 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/24 09:24:43 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

// ===== Buffer Helpers =====
	void 						Client::appendToInbuf(const std::string& data) { _inbuf += data; }		// Add received data
	void						Client::enqueueOutput(const std::string& msg) { _outbuf += msg; }		// Queue data for sending

	std::vector<std::string>	Client::popCompleteLines()
	{
		std::vector<std::string> lines;
		size_t pos;

		// Keep searching for complete messages ending with "\r\n"
		while ((pos = _inbuf.find("\r\n")) != std::string::npos)
		{
			// Extract one complete line (up to \r\n)
			std::string line = _inbuf.substr(0, pos);
			lines.push_back(line);

			// Remove this line (and the "\r\n") from the buffer
			_inbuf.erase(0, pos + 2);
		}

		// Any leftover data (incomplete line) stays in _inbuf
		return lines;
	}

// ===== Channel helpers =====
	void Client::addChannel(const std::string& channel)
	{
		(void)channel;
		_channels.insert(channel);
	}

	void Client::removeChannel(const std::string& channel)
	{
		(void)channel;
		_channels.erase(channel);
	}

// ===== Connection control =====
	void Client::markForClose(){ _closing = true; }

// ===== Messaging =====
	void Client::sendNumericReply(Server &server ,int code, const std::string &arg, const std::string &message)
	{
		std::string			reply;
		std::stringstream	s_code;

		s_code << code;

		// Format: ":server <code> <nickname> <arg> :<message>\r\n"
		reply += "ft_irc ";
		reply += s_code.str() + " ";
		reply += _nick + " ";
		
		if (!arg.empty())
			reply += arg + " ";
		if (!message.empty())
			reply += ":" + message;
		reply += "\r\n";
		
		server.sendMsgToClient(this, reply);
	}
