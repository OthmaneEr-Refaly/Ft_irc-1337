#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>

class Server
{
    private:
    int			listen_fd;
	int			port;
	std::string	password;

    public:
			Server();
	// To Check Later (copy constructor & assignemet operator); Copying a Server can cause a problem (Forbidden). 
			Server(const Server& other);
	Server	&operator=(const Server &other);
			~Server();

			Server(int port, std::string password);
	
};

#endif