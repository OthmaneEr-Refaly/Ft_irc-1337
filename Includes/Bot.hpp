#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <set>
#include <vector>
#include <iostream>
#include "Client.hpp"
#include "Server.hpp"

#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"
#define RESET "\033[0m"

class Bot : public Client
{
    public:
        Bot();
        ~Bot();

        void respondToMessage(Server &server, Client &sender, const std::string &message, const std::string &target);
};

#endif
