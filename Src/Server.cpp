#include "../Includes/Server.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/Channel.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

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
		if (this != &other) {
			_port          = other._port;
			_password      = other._password;
			_listen_fd     = other._listen_fd;
			_poll_fds      = other._poll_fds;
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

// ===== Main control =====

	void Server::run() 
	{
		std::cout << "inside the run function" << std::endl;
		_running = true;

		initListenSocket(); // Initialize the listening socket
		if (!_running)
			return;

		while(_running)
			handlePollEvents(); // Handle events from clients and the server
	} 
	void Server::stop()
	{
	}

// ===== Internal helpers =====
	void Server::initListenSocket() 
	{   
		int socket_fd;
		struct sockaddr_in my_addr;

		socket_fd = socket(AF_INET,SOCK_STREAM,0);
		std::cout << "socket_fd: " << socket_fd << std::endl;
		if (socket_fd == -1)
		{
			std::cerr << "Error creating socket" << std::endl;
			_running = false;
		}
		
		my_addr.sin_family = AF_INET;
		std::cout << "Debugging | sin_famly is =  " << my_addr.sin_family << std::endl;
		my_addr.sin_port = htons(3490);
		std::cout << "Debugging | sin_port is = " << my_addr.sin_port << std::endl;
		my_addr.sin_addr.s_addr = INADDR_ANY;
		std::cout << "Debugging | sin_addr is = " << my_addr.sin_addr.s_addr << std::endl;
		my_addr.sin_zero[0] = '\0';
		std::cout << "Debugging | sin_zero is = " << my_addr.sin_zero[0] << std::endl;
		
		fcntl(socket_fd,F_SETFL,O_NONBLOCK);
		
		//setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){0}, sizeof(int));
		
		int bind_result;
		_listen_fd = socket_fd;
		bind_result = bind(socket_fd, (struct sockaddr*)&my_addr, sizeof(my_addr));
		if (bind_result == -1)
		{
			std::cerr << "Error binding socket" << std::endl;
			_running = false;
			return;
		}
		std::cout << "Socket bound successfully" << std::endl;
		
		int listen_result;
		listen_result = listen(socket_fd,50);
		if (listen_result == -1)
		{
			std::cerr << "Error listening on socket" << std::endl;
			_running = false;
			return;
		}
		
		// =========== [MB] part start ============
		struct pollfd	poll_struct;
		poll_struct.fd = _listen_fd;
		poll_struct.events = POLLIN;
		poll_struct.revents = 0;
		_poll_fds.push_back(poll_struct);
		// =========== [MB] part end. =========
		
		std::cout << "the server is listening" << std::endl; 

	}

	void Server::handlePollEvents()
	{
		if (_poll_fds.empty())
			return ;

		int timeout_ms = 1000; // wait 1000ms = 1s.
		int ready = poll(&_poll_fds[0], _poll_fds.size(), timeout_ms);

		if (ready == -1)
		{
			std::cerr << "poll failed: ";
			perror("poll");
			_running = false;
			return ;
		}

		if (ready == 0)
			return ;

		
		for (int i = 0; i < _poll_fds.size(); i++)
		{
			int fd = _poll_fds[i].fd;
			int result_event = _poll_fds[i].revents;
			
			if (result_event == 0) // no events.
				continue ;

			if (fd == _listen_fd && (result_event & POLLIN)) // 1. New connection on listening socket
			{
				acceptNewClient();
				continue ;
			}
			if (result_event & POLLIN) // 2. Existing client sent data
				handleClientRead(fd);
			if (result_event & POLLOUT)// 3. Existing client ready to send
				handleClientWrite(fd);
			if (re & (POLLER | POLLHUP | POLLNVAL)) // 4. Error or disconnection
				removeClient(fd);
		}
	} 

	void Server::acceptNewClient() { /* TO DO; */ } 
	void Server::removeClient(int fd) { (void)fd; /* TO DO; */ } 
	void Server::handleClientRead(int fd) { (void)fd; /* TO DO; */ }
	void Server::handleClientWrite(int fd) { (void)fd; /* TO DO; */ } 
	void Server::sendToFd(int fd, const std::string& msg) { (void)fd; (void)msg; /* TO DO; */ }
