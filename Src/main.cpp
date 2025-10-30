/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:13:05 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/22 15:34:18 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Headers.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    // --- Validate port ---
    std::string portStr = argv[1];
    if (portStr.find_first_not_of("0123456789") != std::string::npos)
    {
        std::cout << "Please enter a valid port number" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port < 1 || port > 65535)
    {
        std::cout << "The port is out of range (1–65535)" << std::endl;
        return 1;
    }

    // --- Validate password ---
    std::string password = argv[2];
    if (password.length() < 8)
    {
        std::cout << "The password is too short (min 8 chars)" << std::endl;
        return 1;
    }
    if (password.length() > 16)
    {
        std::cout << "The password is too long (max 16 chars)" << std::endl;
        return 1;
    }

    // --- Start server ---
    std::cout << "Server is running on port " << port
              << " with password " << password << std::endl;
    std::srand(std::time(0));
    Server server(port, password);
	server.run();
	server.stop();

    return 0;
}
