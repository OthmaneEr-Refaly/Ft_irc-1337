#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <poll.h> // for pollfd struct
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

class Client;
class Channel;

class Server 
{

	private:
    // ===== Configuration =====
        int				_port;
        std::string		_password;

    // ===== Networking state =====
        int							_listen_fd; // Listening socket fd
        std::vector<struct pollfd>	_poll_fds;  // Poll list
        bool						_running;   // Main loop flag

    // ===== Data structures =====
        std::map<int, Client*>				_fd_to_client;
        std::map<std::string, Client*>		_nick_to_client;
        std::map<std::string, Channel*>		_channels;

    // ===== Internal helpers (defined in .cpp) =====
		void	initListenSocket();
        void	acceptNewClient();
        void	removeClient(int fd);
        void	handlePollEvents();
        void	handleClientRead(int fd);
        void	handleClientWrite(int fd);
        void	sendToFd(int fd, const std::string& msg);


	public:
    // ===== Canonical form =====
                Server();
                Server(int port, const std::string& password);
                Server(const Server& other);
        Server&	operator=(const Server& other);
                ~Server();

    // ===== Getters =====
        int										getPort() const;
        const std::string&						getPassword() const;
        bool									isRunning() const;
        const std::map<int, Client*>&			getFdToClient() const;
        const std::map<std::string, Client*>&	getNickToClient() const;
        const std::map<std::string, Channel*>&	getChannels() const;

    // ===== Setters =====
        void setRunning(bool value);

    // ===== Main control =====
        void run();  // Start the server loop
        void stop(); // Stop the server gracefully
};

#endif
