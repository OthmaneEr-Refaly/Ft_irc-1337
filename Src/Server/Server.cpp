// #include "../Includes/Server.hpp"
// #include "../Includes/Client.hpp"
// #include "../Includes/Channel.hpp"
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <arpa/inet.h>
// #include <sys/types.h>


#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>

// ===== Canonical form =====
	Server::Server()
		: _port(0), _password(""), _listen_fd(-1), _running(false) {}

	Server::Server(int port, const std::string& password)
		: _port(port), _password(password), _listen_fd(-1), _running(false) {}

	Server::Server(const Server& other)
	{
		*this = other;
	}

	Server& Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			_port          = other._port;
			_password      = other._password;
			_listen_fd     = other._listen_fd;
			_pollTable      = other._pollTable;
			_running       = other._running;
			_fd_to_client  = other._fd_to_client;
			_nick_to_client= other._nick_to_client;
			_channels      = other._channels;
		}
		return *this;
	}

	Server::~Server() {}

// ===== Getters =====
	int 									Server::getPort() const { return (_port); }
	const std::string&						Server::getPassword() const { return (_password); }
	bool 									Server::isRunning() const { return (_running); }
	const std::map<int, Client*>&			Server::getFdToClient() const { return (_fd_to_client); }
	const std::map<std::string, Client*>&	Server::getNickToClient() const { return (_nick_to_client); }
	const std::map<std::string, Channel*>&	Server::getChannels() const { return (_channels); }

// ===== Setters =====
	void Server::setRunning(bool value) { _running = value; }


