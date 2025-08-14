
if (argc == 3)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cout << "Failed to create socket" << std::endl;
            return 1;
        }
        //hna after the socket is created, i need to bind it to the port, walakin 9bal i need to define the address of the server.
        sockaddr_in serverAddress; //this is the data type that is used to store the address of the socket.
        serverAddress.sin_family = AF_INET; //this is the family of the address, in this case it's IPv4.    
        serverAddress.sin_port = htons(PortNumber); //this is the port number that the server will listen on.
        //htons()This function is used to convert the unsigned int from machine byte order to network byte order.
        serverAddress.sin_addr.s_addr = INADDR_ANY; //this is the address of the server, in this case it's the local host.
        //INADDR_ANY is a constant that is used to bind the socket to all the interfaces of the machine.
        if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)//hnaya binding the socket to the port.
        {
            std::cout << "Failed to bind socket" << std::endl;
        }
        listen(serverSocket, 10);
        std::cout << "Server is listening on port " << PortNumber << std::endl;
        while (true)
        {
            int clientSocket = accept(serverSocket, NULL, NULL);
        }
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        send(clientSocket, "Message received", 18, 0);
        close(clientSocket);
        close(serverSocket);
        return 0;
}