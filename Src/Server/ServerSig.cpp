/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:24:33 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/21 09:28:34 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

Server* Server::g_instance = NULL;

void Server::handleSignal(int signum)
{
	 if (g_instance)
    {
        std::cout << "\nReceived signal " << signum << ", stopping server..." << std::endl;
        g_instance->stop();
    }
}
