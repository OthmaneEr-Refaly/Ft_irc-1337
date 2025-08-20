#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../Includes/Client.hpp"
#include "../Includes/Channel.hpp"
#include "../Includes/Server.hpp"


int main(int argc, char **argv)
{
	if(argc == 3)
	{
		std::string str = argv[1];
		if (!(str.find_first_not_of("0123456789") == std::string::npos))
		{
			std::cout << "Please enter a valid port number" << std::endl;
			return 1;
		}
		//i think i need to check tha max int here.
		int PortNumber = std::atoi(argv[1]);
		if (PortNumber < 1 || PortNumber > 65535)
		{
			std::cout << "The port is out of range" << std::endl;
			return 1;
		}
		std::string Password = argv[2];
		//This Lenght check is completely optional, but i think it's a good idea to check it.
		if (Password.length() < 8)
		{
			std::cout << "The password is too short" << std::endl;
			return 1;
		}
		if (Password.length() > 16)
		{
			std::cout << "The password is too long" << std::endl;
			return 1;
		}
		std::cout << "Server is running on port " << PortNumber << " with password " << Password << std::endl;
	
    	// Create Server object
    	Server server(6667, "secret");

    	// Create some Client objects
    	Client client1;
    	Client client2(42); // example constructor with fd if you have it

    	// Create some Channel objects
    	// Channel channel1;
    	// Channel channel2("#general");

    	// Avoid unused variable warnings
    	(void)server;
    	(void)client1;
    	(void)client2;
    	// (void)channel1;
    	// (void)channel2;

    std::cout << "Headers and .cpp files compile correctly!" << std::endl;
	//i thin hna i need to call the run function ??
		server.run();
	}
	else
	{
		std::cout << "Please enter a valid port number and a password" << std::endl;
		return 1;
	}
	return 0;
}


