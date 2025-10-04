/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:41:48 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 15:53:45 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

// ===== Canonical form =====
Client::Client()
	: _fd(-1), _closing(false), _registered(false), _passOk(false) {}

Client::Client(int fd)
	: _fd(fd), _closing(false), _registered(false), _passOk(false) {}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_inbuf = other._inbuf;
		_outbuf = other._outbuf;
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

Client::~Client() {}

// ===== Getters =====
int Client::getFd() const { return (_fd); }
const std::string &Client::getInbuf() const { return (_inbuf); }
const std::string &Client::getOutbuf() const { return (_outbuf); }
std::string &Client::getOutbufRef() { return (_outbuf); }
bool Client::isRegistered() const { return (_registered); }
bool Client::isClosing() const { return (_closing); }
bool Client::isPassOk() const { return (_passOk); }
const std::string &Client::getNick() const { return (_nick); }
const std::string &Client::getUser() const { return (_user); }
const std::string &Client::getRealname() const { return (_realname); }
const std::string &Client::getHost() const { return (_host); }
const std::set<std::string> &Client::getChannels() const { return (_channels); }

// ===== Setters =====
void Client::setFd(int fd) { _fd = fd; }
void Client::setInbuf(const std::string &buf) { _inbuf = buf; }
void Client::setOutbuf(const std::string &buf) { _outbuf = buf; }
void Client::setRegistered(bool value) { _registered = value; }
void Client::setPassOk(bool value) { _passOk = value; }
void Client::setNick(const std::string &nick) { _nick = nick; }
void Client::setUser(const std::string &user) { _user = user; }
void Client::setRealname(const std::string &realname) { _realname = realname; }
void Client::setHost(const std::string &host) { _host = host; }
void Client::setChannels(const std::set<std::string> &channels) { _channels = channels; }
