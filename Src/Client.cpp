/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:41:48 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/19 17:14:59 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"
#include <unistd.h>
#include <sstream>

// ===== Canonical form =====
	Client::Client() 
		: _fd(-1), _want_write(false), _closing(false), _registered(false), _passOk(false) {}

	Client::Client(int fd)
		: _fd(fd), _want_write(false), _closing(false), _registered(false), _passOk(false) {}

	Client::Client(const Client& other)
	{
		*this = other;
	}

	Client& Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			_fd = other._fd;
			_inbuf = other._inbuf;
			_outbuf = other._outbuf;
			_want_write = other._want_write;
			_registered = other._registered;
			_passOk = other._passOk;
			_closing = other._closing;
			_nick = other._nick;
			_user = other._user;
			_realname = other._realname;
			_host = other._host;
			_channels = other._channels; 
		}
		return (*this);
	}

	Client::~Client(){}

// ===== Getters =====
	int								Client::getFd() const { return (_fd); }
	const std::string&				Client::getInbuf() const { return (_inbuf); }
	const std::string&				Client::getOutbuf() const { return (_outbuf); }
	bool							Client::getWantsWrite() const { return (_want_write); }
	bool 							Client::isRegistered() const { return (_registered); }
	bool 							Client::isClosing() const { return (_closing); }
	bool 							Client::isPassOk() const { return (_passOk); }
	const std::string&				Client::getNick() const { return (_nick); }
	const std::string&				Client::getUser() const { return (_user); }
	const std::string&				Client::getRealname() const { return (_realname); }
	const std::string&				Client::getHost() const { return (_host); }
	const std::set<std::string>&	Client::getChannels() const { return (_channels); }

// ===== Setters =====
	void Client::setFd(int fd) { _fd = fd; }
	void Client::setInbuf(const std::string& buf) { _inbuf = buf; }
	void Client::setOutbuf(const std::string& buf) { _outbuf = buf; }
	void Client::setWantsWrite(bool value) { _want_write = value; }
	void Client::setRegistered(bool value) { _registered = value; }
	void Client::setPassOk(bool value) { _passOk = value; }
	void Client::setNick(const std::string& nick) { _nick = nick; }
	void Client::setUser(const std::string& user) { _user = user; }
	void Client::setRealname(const std::string& realname) { _realname = realname; }
	void Client::setHost(const std::string& host) { _host = host; }
	void Client::setChannels(const std::set<std::string>& channels) { _channels = channels; }

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

	void Client::sendNumericReply(int code, const std::string &arg, const std::string &message)
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
		
		write(_fd, reply.c_str(), reply.size());
	}

void Client::sendMessage(const std::string& message)
{
	enqueueOutput(message + "\r\n");
	_want_write = true; // Ensure we want to write when there's data to send
}
