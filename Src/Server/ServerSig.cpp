/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:24:33 by mobouifr          #+#    #+#             */
/*   Updated: 2025/11/04 09:03:52 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

Server* Server::g_instance = NULL;
bool Server::_siged = false;

void Server::handleSignal(int signum)
{
	 if (g_instance)
    {
        _siged = true;
        std::cout << "\nReceived signal " << signum << ", stopping server..." << std::endl;
        g_instance->stop();
    }
}
