/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:43:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/02 16:24:51 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>


void Server::initListenSocket()
{
    int					option_value;
    int					socket_fd;
    struct sockaddr_in	my_addr;
    int					bind_result;
    int					listen_result;

    option_value = 1;

    // socket :
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "socket_fd: " << socket_fd << std::endl;
    if (socket_fd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        _running = false;
    }


    // initializing sockaddr_in structure :
    my_addr.sin_family = AF_INET;
    std::cout << "Debugging | sin_famly is =  " << my_addr.sin_family << std::endl;
    std::cout << "the port is = " << _port << std::endl;
    my_addr.sin_port = htons(_port);
    std::cout << "Debugging | sin_port is = " << my_addr.sin_port << std::endl;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    std::cout << "Debugging | sin_addr is = " << my_addr.sin_addr.s_addr << std::endl;
    my_addr.sin_zero[0] = '\0';
    std::cout << "Debugging | sin_zero is = " << my_addr.sin_zero[0] << std::endl;


    // fcntl and setsockopt :
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (&option_value),
        sizeof(option_value));


    //	bind and listen :
    _listen_fd = socket_fd;
    bind_result = bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (bind_result == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        _running = false;
        return ;
    }
    std::cout << "Socket bound successfully" << std::endl;
    
    listen_result = listen(socket_fd, 50);
    if (listen_result == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        _running = false;
        return ;
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

	