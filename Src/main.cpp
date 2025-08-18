// #include <iostream>
// #include <cstdlib>
// #include <sys/socket.h>
// #include <cstring>
// #include <iostream>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <cstring>
// #include <iostream>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>


// int main(int argc, char **argv)
// {
//     if(argc == 3)
//     {
//         std::string str = argv[1];
//         if (!(str.find_first_not_of("0123456789") == std::string::npos))
//         {
//             std::cout << "Please enter a valid port number" << std::endl;
//             return 1;
//         }
//         //i think i need to check tha max int here.
//         int PortNumber = std::atoi(argv[1]);
//         if (PortNumber < 1 || PortNumber > 65535)
//         {
//             std::cout << "The port is out of range" << std::endl;
//             return 1;
//         }
//         std::string Password = argv[2];
//         //This Lenght check is completely optional, but i think it's a good idea to check it.
//         if (Password.length() < 8)
//         {
//             std::cout << "The password is too short" << std::endl;
//             return 1;
//         }
//         if (Password.length() > 16)
//         {
//             std::cout << "The password is too long" << std::endl;
//             return 1;
//         }
//         std::cout << "Server is running on port " << PortNumber << " with password " << Password << std::endl;
//         //right here im creating the socket.
//         int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//         if (serverSocket == -1)
//         {
//             std::cout << "Failed to create socket" << std::endl;
//             return 1;
//         }
//         //hna after the socket is created, i need to bind it to the port, walakin 9bal i need to define the address of the server.
//         sockaddr_in serverAddress; //this is the data type that is used to store the address of the socket.
//         serverAddress.sin_family = AF_INET; //this is the family of the address, in this case it's IPv4.    
//         serverAddress.sin_port = htons(PortNumber); //this is the port number that the server will listen on.
//         //htons()This function is used to convert the unsigned int from machine byte order to network byte order.
//         serverAddress.sin_addr.s_addr = INADDR_ANY; //this is the address of the server, in this case it's the local host.
//         //INADDR_ANY is a constant that is used to bind the socket to all the interfaces of the machine.
//         if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)//hnaya binding the socket to the port.
//         {
//             std::cout << "Failed to bind socket" << std::endl;
//         }
//         listen(serverSocket, 10);
//         std::cout << "Server is listening on port " << PortNumber << std::endl;
//         while (true)
//         {
//             int clientSocket = accept(serverSocket, NULL, NULL);
//         }
//         char buffer[1024] = {0};
//         recv(clientSocket, buffer, sizeof(buffer), 0);
//         std::cout << "Message from client: " << buffer << std::endl;
//         send(clientSocket, "Message received", 18, 0);
//         close(clientSocket);
//         close(serverSocket);
//         return 0;
//         //now same thing for the client.
//         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//         if (clientSocket == -1)
//         {
//             std::cout << "Failed to create socket" << std::endl;
//             return 1;
//         }
//         sockaddr_in clientAddress;
//         socklen_t clientAddressLength = sizeof(clientAddress);
//         int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
//         if (clientSocket == -1)
//         {
//             std::cout << "Failed to accept client" << std::endl;
//         }
//         char buffer[1024] = {0};
//         recv(clientSocket, buffer, sizeof(buffer), 0);
//         std::cout << "Message from client: " << buffer << std::endl;
//         send(clientSocket, "Message received", 18, 0);
//         close(clientSocket);
//         close(serverSocket);
//         return 0;

//     }
//     else
//     {
//         std::cout << "Please enter a valid port number and a password" << std::endl;
//         return 1;
//     }
//     return 0;
// }

#include "../Includes/Client.hpp"
#include "../Includes/Channel.hpp"
#include "../Includes/Server.hpp"

#include <iostream>

int main() {
    // === Test Client ===
    Client c1(10);  // fd = 10
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
    Server s1(6667, "pass123");
    std::cout << "Server running? " << (s1.isRunning() ? "yes" : "no") << "\n";
    s1.setRunning(true);
    std::cout << "Server running? " << (s1.isRunning() ? "yes" : "no") << "\n";
    std::cout << "Server port: " << s1.getPort() << "\n";
    std::cout << "Server password: " << s1.getPassword() << "\n";

    return 0;
}

