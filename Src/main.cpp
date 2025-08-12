#include <iostream>
#include <cstdlib>


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
    }
    else
    {
        std::cout << "Please enter a valid port number and a password" << std::endl;
        return 1;
    }
    return 0;
}