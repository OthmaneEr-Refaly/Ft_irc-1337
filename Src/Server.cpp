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

// ===== Project headers =====
#include "../Includes/Server.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/Channel.hpp"

// ===== C++ standard headers =====
#include <iostream>      // std::cout, std::cerr
#include <string>        // std::string
#include <vector>        // std::vector
#include <map>           // std::map
#include <cstring>       // std::strerror, std::memset

// ===== POSIX / system headers =====
#include <sys/types.h>   // basic system types (socklen_t, etc.)
#include <sys/socket.h>  // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h>  // sockaddr_in, htons, INADDR_ANY
#include <arpa/inet.h>   // inet_ntoa, ntohs, etc. (if needed)
#include <unistd.h>      // close()
#include <fcntl.h>       // fcntl()
#include <poll.h>        // poll(), struct pollfd
#include <errno.h>       // errno
#include <stdio.h>       // perror()

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
		std::cout << "Debugging | sin_family is =  " << my_addr.sin_family << std::endl;
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
		/*****************************************************************************/
		/* Function: handlePollEvents 												 */
		/* How it works : 												 			 */
		/*   1. Poll all fds in _poll_fds with timeout 1000ms.                       */
		/*   2. Iterate through poll results:                                        */
		/*      - Listening socket ready -> acceptNewClient()                        */
		/*      - Client fd ready for read -> handleClientRead(fd)                   */
		/*      - Client fd ready for write -> handleClientWrite(fd)                 */
		/*      - Error / disconnect -> removeClient(fd)                             */
		/* Key points:                                                               */
		/*   - Ensure _poll_fds contains listening socket.                           */
		/*   - poll() should not block indefinitely.                                 */
		/*****************************************************************************/

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

		
		for (size_t i = 0; i < _poll_fds.size(); i++)
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
			if (result_event & POLLIN) 	// 2. Existing client sent data
				handleClientRead(fd);
			if (result_event & POLLOUT) // 3. Existing client ready to send
				handleClientWrite(fd);
			if (result_event & (POLLERR | POLLHUP | POLLNVAL)) // 4. Error or disconnection
				removeClient(fd);
		}
	} 

	void Server::acceptNewClient()
	{
		/*****************************************************************************/
		/* Function: acceptNewClient                                                 */
		/* How it works:                                                             */
		/*   1. Accept a new client connection from the listening socket.            */
		/*   2. Set the new socket to non-blocking mode.                             */
		/*   3. Add a pollfd entry to _poll_fds.                                     */
		/*   4. Create a new Client object and store it in _fd_to_client.            */
		/* Key points:                                                               */
		/*   - Log new connections for debugging.                                    */
		/*   - New client should appear in both _fd_to_client and _poll_fds.         */
		/*****************************************************************************/

		struct sockaddr_in client_addr;
		socklen_t addr_len = sizeof(client_addr);

		int client_fd = accept(_listen_fd, (struct sockaddr*)&client_addr, &addr_len);
		if (client_fd == -1)
		{
			perror("accept");
			return;
		}

		fcntl(client_fd, F_SETFL, O_NONBLOCK);

		struct pollfd pfd;
		pfd.fd = client_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;  
		_poll_fds.push_back(pfd);

		Client* new_client = new Client(client_fd);
		_fd_to_client[client_fd] = new_client;

		std::cout << "New client connected (fd=" << client_fd << ")" << std::endl;

	}
	
	void Server::handleClientRead(int fd)
	{
		/*****************************************************************************/
		/* Function: handleClientRead                                                */
		/* How it works:                                                             */
		/*   1. Read up to 512 bytes from client socket using recv().                */
		/*   2. Handle disconnection (bytes_read == 0) or error (bytes_read < 0).    */
		/*   3. Append received data to client's _inbuf.                             */
		/*   4. Extract complete lines using popCompleteLines().                     */
		/* Key points:                                                               */
		/*   - Leftover incomplete lines remain in _inbuf.                           */
		/*   - Call removeClient(fd) on disconnect or error.                         */
		/*****************************************************************************/

		// Find client by fd
		std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
		if (it == _fd_to_client.end())
			return; // client not found, maybe already removed

		Client &client = *(it->second);

		char	buffer[512];  // IRC recommends max line length = 512 bytes
		ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

		if (bytes_read <= 0)
		{
			if (bytes_read == 0) // Client disconnected (0)
				std::cout << "Client " << fd << " disconnected" << std::endl;
			else // Error (-1)
				std::cerr << "recv() error on fd " << fd << ": "
						<< strerror(errno) << std::endl;

			removeClient(fd);
			return;
		}

		buffer[bytes_read] = '\0';

		std::string data(buffer);
		client.appendToInbuf(data);

		std::vector<std::string> lines = client.popCompleteLines(); // Extract complete lines (commands ending with "\r\n")

		for (size_t i = 0; i < lines.size(); i++)
		{
			std::cout << "Parsed command from fd " << fd << ": [" << lines[i] << "]" << std::endl;

			// Later: pass to a "parseCommand(client, lines[i])" function
			// split line into tokens, check if it's NICK, USER, JOIN, PRIVMSG, etc.
		}
	}

	void Server::handleClientWrite(int fd)
	{
		/*****************************************************************************/
		/* Function: handleClientWrite                                               */
		/* How it works:                                                             */
		/*   1. Check if client's _outbuf is empty -> stop POLLOUT monitoring.       */
		/*   2. Send data from _outbuf using send().                                 */
		/*   3. Remove the sent portion from _outbuf.                                */
		/* Key points:                                                               */
		/*   - Handle EAGAIN/EWOULDBLOCK/EINTR errors by retrying later.             */
		/*   - Stop POLLOUT when _outbuf becomes empty.                              */
		/*****************************************************************************/

		// Find the client by fd
		std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
		if (it == _fd_to_client.end())
			return; // if no more client return

		Client* client = it->second;
		std::string &out = const_cast<std::string&>(client->getOutbuf());

		// If Nothing to send → stop listening for POLLOUT
		if (out.empty()) 
		{
			disableWriteInterest(fd);
			client->setWantsWrite(false);
			return;
		}

		// Try to send from the beginning of _outbuf
		#ifdef MSG_NOSIGNAL
			ssize_t n = send(fd, out.data(), out.size(), MSG_NOSIGNAL);
		#else
			ssize_t n = send(fd, out.data(), out.size(), 0);
		#endif

		if (n > 0) 
		{
			// Remove the part we just sent
			out.erase(0, static_cast<size_t>(n));

			if (out.empty()) 
			{
				// When everything is sent stop POLLOUT
				disableWriteInterest(fd);
				client->setWantsWrite(false);
			}
			return;
		}

		// Error handling
		if (n == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			{
				// Not a fatal error, just try again later
				client->setWantsWrite(true);
				return;
			}
			
			if (errno == EPIPE || errno == ECONNRESET)
			{
				// Client disconnected
				removeClient(fd);
				return;
			}

			// Unexpected error, log + cleanup
			std::cerr << "send() error on fd " << fd << ": " << strerror(errno) << std::endl;
			removeClient(fd);
			return;
		}

		// PS: n == 0 is rare for send(), treat like "nothing sent"
	}

	void Server::removeClient(int fd)
	{
		/*****************************************************************************/
		/* Function: removeClient                                                    */
		/* How it works:                                                             */
		/*   1. Close the client socket.                                             */
		/*   2. Remove fd from _poll_fds.                                            */
		/*   3. Delete Client object and remove from _fd_to_client.                  */
		/*   4. Remove from _nick_to_client map if client was registered.            */
		/* Key points:                                                               */
		/*   - Prevent memory leaks by deleting Client objects.                      */
		/*   - Ensure fd is no longer monitored in _poll_fds.                        */
		/*****************************************************************************/

		std::cout << "Removing client fd=" << fd << std::endl;

		// 1. Close the socket
		close(fd);

		// 2. Remove from poll fds
		int idx = findPollIndex(fd);
		if (idx != -1)
			_poll_fds.erase(_poll_fds.begin() + idx);

		// 3. Remove from fd_to_client and delete client object
		std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
		if (it != _fd_to_client.end())
		{
			delete it->second;
			_fd_to_client.erase(it);
		}

		// 4. Remove from nick_to_client
		for (std::map<std::string, Client*>::iterator nit = _nick_to_client.begin(); nit != _nick_to_client.end(); ++nit)
		{
			if (nit->second->getFd() == fd)
			{
				_nick_to_client.erase(nit);
				break;
			}
		}
		std::cout << "Client fd=" << fd << " removed successfully" << std::endl;
	}

	void Server::sendToFd(int fd, const std::string& msg) { (void)fd; (void)msg; /* TO DO; */ }

// ===== New Poll helper functions =====
	int		Server::findPollIndex(int fd) const
	{
		/*****************************************************************************/
		/* Function: findPollIndex                                                   */
		/* How it works:                                                             */
		/*   1. Iterate through _poll_fds vector.                                    */
		/*   2. Compare each fd with the input fd.                                   */
		/*   3. Return index if found, -1 if not found.                              */
		/* Key points:                                                               */
		/*   - Used internally to enable/disable POLLOUT for specific fd.            */
		/*****************************************************************************/

		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			if (_poll_fds[i].fd == fd) 
				return static_cast<int>(i);
		}
		return (-1);
	}

	void	Server::enableWriteInterest(int fd)
	{
		/*****************************************************************************/
		/* Function: enableWriteInterest                                             */
		/* How it works:                                                             */
		/*   1. Find fd index in _poll_fds using findPollIndex().                    */
		/*   2. Set POLLOUT bit in events for that fd.                               */
		/* Key points:                                                               */
		/*   - Allows handleClientWrite to be triggered when fd is writable.         */
		/*****************************************************************************/

		int idx = findPollIndex(fd);
    	if (idx >= 0)
			_poll_fds[idx].events |= POLLOUT;
	}

	void	Server::disableWriteInterest(int fd)
	{
		/*****************************************************************************/
		/* Function: disableWriteInterest                                            */
		/* How it works:                                                             */
		/*   1. Find fd index in _poll_fds using findPollIndex().                    */
		/*   2. Clear POLLOUT bit in events for that fd.                             */
		/* Key points:                                                               */
		/*   - Stops unnecessary POLLOUT events once all data has been sent.         */
		/*****************************************************************************/

		int idx = findPollIndex(fd);
		if (idx >= 0)
			_poll_fds[idx].events &= ~POLLOUT;
	}
