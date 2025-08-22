#include "../Includes/Channel.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int	PortNumber;

	if (argc == 3)
	{
		std::string str = argv[1];
		if (!(str.find_first_not_of("0123456789") == std::string::npos))
		{
			std::cout << "Please enter a valid port number" << std::endl;
			return (1);
		}
		// i think i need to check tha max int here.
		PortNumber = std::atoi(argv[1]);
		if (PortNumber < 1 || PortNumber > 65535)
		{
			std::cout << "The port is out of range" << std::endl;
			return (1);
		}
		std::string Password = argv[2];
		// This Lenght check is completely optional,but i think it's a good idea to check it.
		if (Password.length() < 8)
		{
			std::cout << "The password is too short" << std::endl;
			return (1);
		}
		if (Password.length() > 16)
		{
			std::cout << "The password is too long" << std::endl;
			return (1);
		}
		std::cout << "Server is running on port " << PortNumber << " with password " << Password << std::endl;
		std::cout << "Headers and .cpp files compile correctly!" << std::endl;
		// i thin hna i need to call the run function ??
		Client c1(10); // fd = 10
		c1.setNick("Montassir");
		c1.setUser("mb");
		c1.setRealname("Montassir Bouifraden");
		c1.setHost("127.0.0.1");
		c1.setRegistered(true);
		std::cout << "Client nick: " << c1.getNick() << "\n";
		std::cout << "Registered: " << (c1.isRegistered() ? "yes" : "no") << "\n";
		// Buffer test
		c1.appendToInbuf("Hello World\r\n");
		c1.enqueueOutput("Welcome!\r\n");
		std::cout << "Inbuf: " << c1.getInbuf() << "\n";
		std::cout << "Outbuf: " << c1.getOutbuf() << "\n";
		// === Test Channel ===
		Channel ch1("#42");
		ch1.setTopic("Welcome to ft_irc!");
		ch1.addMember(&c1);
		ch1.addOperator(&c1);
		ch1.addInvite("Othman");
		std::cout << "Channel name: " << ch1.getName() << "\n";
		std::cout << "Topic: " << ch1.getTopic() << "\n";
		std::cout << "Is Montassir member? " << (ch1.isMember(&c1) ? "yes" : "no") << "\n";
		std::cout << "Is Montassir operator? " << (ch1.isOperator(&c1) ? "yes" : "no") << "\n";
		std::cout << "Is Othman invited? " << (ch1.isInvited("Othman") ? "yes" : "no") << "\n";
		// === Test Server (basic config only) ===
		Server s1(PortNumber, Password);
		std::cout << "Server running? " << (s1.isRunning() ? "yes" : "no") << "\n";
		s1.setRunning(true);
		std::cout << "Server running? " << (s1.isRunning() ? "yes" : "no") << "\n";
		std::cout << "Server port: " << s1.getPort() << "\n";
		std::cout << "Server password: " << s1.getPassword() << "\n";
		s1.run();
	}
	else
	{
		std::cout << "Please enter a valid port number and a password" << std::endl;
		return (1);
	}
	return (0);
}
