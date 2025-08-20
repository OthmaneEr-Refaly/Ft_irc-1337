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

	void Server::run() {
	std::cout << "inside the run function" << std::endl;
	_running = true;
	while(_running) {
		initListenSocket(); // Initialize the listening socket
		handlePollEvents(); // Handle events from clients and the server
	}
} 
	void Server::stop()
	{
	}

// ===== Internal helpers =====
	void Server::initListenSocket() {   
	int socket_fd;
	struct sockaddr_in my_addr;
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	std::cout << "socket_fd: " << socket_fd << std::endl;
	if (socket_fd == -1) {
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
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	int bind_result;
	_listen_fd = socket_fd;
	bind_result = bind(socket_fd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if (bind_result == -1) {
		std::cerr << "Error binding socket" << std::endl;
		_running = false;
		return;
	}
	std::cout << "Socket bound successfully" << std::endl;
	int listen_result;
	listen_result = listen(socket_fd,50);
	if (listen_result == -1) {
		std::cerr << "Error listening on socket" << std::endl;
		_running = false;
		return;
	}
	std::cout << "the server is listening" << std::endl;
}

	void Server::acceptNewClient() { /* TO DO; */ } 
	void Server::removeClient(int fd) { /* TO DO; */ } 
	void Server::handlePollEvents() { /* TO DO; */ } 
	void Server::handleClientRead(int fd) { /* TO DO; */ }
	void Server::handleClientWrite(int fd) { /* TO DO; */ } 
	void Server::sendToFd(int fd, const std::string& msg) { /* TO DO; */ }
