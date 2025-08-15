/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:41:48 by mobouifr          #+#    #+#             */
/*   Updated: 2025/08/15 11:43:44 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

// ===== Canonical form =====
	Client::Client() 
		: _fd(-1), _want_write(false), _registered(false) {}

	Client::Client(int fd)
		: _fd(fd), _want_write(false), _registered(false) {}

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
			_nick = other._nick;
			_user = other._user;
			_realname = other._realname;
			_host = oter._host;
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
	const std::string&				Client::getNick() const { return (_nick); }
	const std::string&				Client::getUser() const { return (User); }
	const std::string&				Client::getRealname() const { return (_realname); }
	const std::string&				Client::getHost() const { return (_host); }
	const std::set<std::string>&	Client::getChannels() const { return (_channels); }

// ===== Setters =====
	void Client::setFd(int fd) { _fd = fd; }
	void Client::setInbuf(const std::string& buf) { _inbuf = buf; }
	void Client::setOutbuf(const std::string& buf) { _outbuf = buf; }
	void Client::setWantsWrite(bool value) { _want_write = value; }
	void Client::setRegistered(bool value) { _registered = value; }
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

		// ===== Goal =====
		// Take all complete IRC protocol messages from _inbuf,
		// store them into 'lines', remove them from _inbuf, and return them.
		//
		// IRC messages are text lines ending with "\r\n".
		// Example: "NICK montassir\r\nUSER user 0 * :Real Name\r\n"

		// ===== Step 1: Search for "\r\n" in _inbuf =====
		// While we can find a "\r\n" in _inbuf:
		//   - Find its position (pos)
		//   - Everything before pos is a complete line (without the "\r\n")

		// ===== Step 2: Extract the line =====
		// Use substr(0, pos) to get the line content.
		// Push it into the 'lines' vector.

		// ===== Step 3: Remove the processed line from _inbuf =====
		// Erase from _inbuf the characters up to pos + 2
		// (+2 to remove the "\r\n" itself).

		// ===== Step 4: Repeat until no "\r\n" is found =====
		// This way, if multiple commands arrived in one read(),
		// we handle all of them in one call.

		// ===== Step 5: Return the vector of complete lines =====
		return lines;

	}

// ===== Channel helpers =====
	void addChannel(const std::string& channel)
	{
		_channels.insert(channel);
	}

	void removeChannel(const std::string& channel)
	{
		_channels.erase(channel);
	}

// ===== Connection control =====
void Client::markForClose()
{
	// Will be implemented when server connection closing is handled
	// e.g., set a "closing" flag or manipulate _fd
}
