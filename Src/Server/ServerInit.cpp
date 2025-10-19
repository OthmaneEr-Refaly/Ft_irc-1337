/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:43:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/13 08:38:32 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

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
    my_addr.sin_port = htons(_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_zero[0] = '\0';

    // fcntl and setsockopt :
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (&option_value),
        sizeof(option_value));

    //	bind and listen :
    _listen_fd = socket_fd;
    bind_result = bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (bind_result == -1)
    {
        _running = false;
        return ;
    }
    
    listen_result = listen(socket_fd, 50);
    if (listen_result == -1)
    {
        _running = false;
        return ;
    }

	struct pollfd	poll_struct;
	poll_struct.fd = _listen_fd;
	poll_struct.events = POLLIN;
	poll_struct.revents = 0;
	_pollTable.push_back(poll_struct);

    std::cout << "the server is listening" << std::endl;

	initCommandMap();
}

	
