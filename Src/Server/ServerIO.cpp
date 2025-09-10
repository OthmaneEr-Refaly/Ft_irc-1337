/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIO.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:50:54 by mobouifr          #+#    #+#             */
/*   Updated: 2025/09/02 16:02:58 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/Server.hpp"
#include "../../Includes/Client.hpp"
#include "../../Includes/Channel.hpp"
#include "../../Includes/Headers.hpp"
#include <cctype>
#include <cstddef>
#include <ctype.h>

void Server::acceptNewClient()
{
	/*****************************************************************************/
	/* Function: acceptNewClient                                                 */
	/* How it works:                                                             */
	/*   1. Accept a new client connection from the listening socket.            */
	/*   2. Set the new socket to non-blocking mode.                             */
	/*   3. Add a pollfd entry to _poll_fds.                                     */
	/*   4. Create a new Client object and store it in _fd_to_client.            */
	/* Key points:                                                               */
	/*   - Log new connections for debugging.                                    */
	/*   - New client should appear in both _fd_to_client and _poll_fds.         */
	/*****************************************************************************/

	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	int client_fd = accept(_listen_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (client_fd == -1)
	{
		perror("accept");
		return;
	}

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;  
	_poll_fds.push_back(pfd);

	Client* new_client = new Client(client_fd);
	_fd_to_client[client_fd] = new_client;

	std::cout << "New client connected (fd=" << client_fd << ")" << std::endl;

}

void Server::handleClientRead(int fd)
{
	/*****************************************************************************/
	/* Function: handleClientRead                                                */
	/* How it works:                                                             */
	/*   1. Read up to 512 bytes from client socket using recv().                */
	/*   2. Handle disconnection (bytes_read == 0) or error (bytes_read < 0).    */
	/*   3. Append received data to client's _inbuf.                             */
	/*   4. Extract complete lines using popCompleteLines().                     */
	/* Key points:                                                               */
	/*   - Leftover incomplete lines remain in _inbuf.                           */
	/*   - Call removeClient(fd) on disconnect or error.                         */
	/*****************************************************************************/

	// Find client by fd
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it == _fd_to_client.end())
		return; // client not found, maybe already removed

	Client &client = *(it->second);

	char	buffer[512];  // IRC recommends max line length = 512 bytes
	ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_read <= 0)
	{
		if (bytes_read == 0) // Client disconnected (0)
			std::cout << "Client " << fd << " disconnected" << std::endl;
		else // Error (-1)
			std::cerr << "recv() error on fd " << fd << ": "
					<< strerror(errno) << std::endl;

		removeClient(fd);
		return;
	}

	buffer[bytes_read] = '\0';

	std::string data(buffer);
	client.appendToInbuf(data);

	std::vector<std::string> lines = client.popCompleteLines(); // Extract complete lines (commands ending with "\r\n")
	std::cout << lines.size() << std::endl;
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::cout << "Parsed command from fd " << fd << ": [" << lines[i] << "]" << std::endl;
		Command cmd = parseRawLine(lines[i]);
	}


	// ===== TEST: send a reply back =====
	client.enqueueOutput("Hello from server!\r\n");
	enableWriteInterest(fd);
}


// Server::Command	Server::parseRawLine(const std::string &line){
	// Command cmd;
	// std::string save_params;
// //	NICK Othmane

// //USER oth 0 * :Othmane Real Name
	// //:nick PRIVMSG #chan aftro :hello world
	// //Expected params = [#chan, aftro, hello world]
	// for(size_t i = 0; i < line.size(); i++)
	// {
		// if (line[0] == ':')
		// {
			// i++;
			// for(; i < line.size() && line[i] != ' '; i++)
				// cmd.prefix += line[i];
			// i++;
		// }
		// else
			// cmd.prefix = "";
		// std::cout << "the prefix is =" << cmd.prefix << std::endl;
		
		// for(; i < line.size() && line[i] != ' '; i++)
			// cmd.command += line[i];
		// for(size_t j = 0; j < cmd.command.size(); j++)
			// cmd.command[j] = toupper(cmd.command[j]);
		// std::cout << "the command is :" << cmd.command << std::endl;
		
		// for(; i < line.size(); i++) {
			// if(line[i] == ':')
			// {
				// i++;
				// for(; i < line.size(); i++)
					// save_params += line[i];
				// cmd.params.push_back(save_params);
				// save_params = "";
			// }
			// else
				// for(;i < line.size() && line[i] != ' '; i++)
						// save_params += line[i];
			// cmd.params.push_back(save_params);
			// save_params = "";
		// }
		// //printing cmd.params
		// for(size_t k = 0; k < cmd.params.size(); k++)
			// std::cout << "the params are :" << cmd.params[k] << std::endl;
	// }
	// return cmd;
// }

Server::Command Server::parseRawLine(const std::string &line) {
    Command cmd;
    size_t pos = 0;
    size_t len = line.size();

    // ===== STEP 1: Parse Prefix (optional) =====
    if (pos < len && line[pos] == ':') {
        size_t spacePos = line.find(' ', pos);
        if (spacePos == std::string::npos) {
            return cmd; // malformed line
        }
        cmd.prefix = line.substr(pos + 1, spacePos - pos - 1);
        pos = spacePos + 1;
    }
	std::cout << "this is the prifex" << cmd.prefix << std::endl;
    // Skip spaces after prefix
    while (pos < len && line[pos] == ' ') 
        pos++;

    // ===== STEP 2: Parse Command (required) =====
    size_t spacePos = line.find(' ', pos);
    if (spacePos == std::string::npos) {
        cmd.command = line.substr(pos);
		pos = std::string::npos;
    } else {
        cmd.command = line.substr(pos, spacePos - pos);
        pos = spacePos + 1;
    }
    // Convert command to uppercase
    for (size_t i = 0; i < cmd.command.size(); i++) {
        cmd.command[i] = std::toupper(cmd.command[i]);
    }
	std::cout << "this is the command " << cmd.command << std::endl;

    // ===== STEP 3: Parse Parameters =====
    // Skip spaces before parameters
    while (pos < len && line[pos] == ' ') 
        pos++;
    // Check if no parameters remain
    if (pos >= len || pos == 0) {
        cmd.params.clear();
        return cmd;
    }

    // Parse all parameters
    while (pos < len) {
        // Handle trailing parameter (starts with ':')
        if (line[pos] == ':') {
            cmd.params.push_back(line.substr(pos + 1));
            break;
        }
        // Handle regular parameter
        size_t nextSpace = line.find(' ', pos);
        if (nextSpace == std::string::npos) {
            // Last parameter
            cmd.params.push_back(line.substr(pos));
            break;
        } else {
            // Middle parameter
            cmd.params.push_back(line.substr(pos, nextSpace - pos));
            pos = nextSpace + 1;
            // Skip multiple spaces
            while (pos < len && line[pos] == ' ') 
                pos++;
        }
    }

    // Debug output for parameters
    for (size_t i = 0; i < cmd.params.size(); i++) {
        std::cout << "this is the param[" << i << "] = " << cmd.params[i] << std::endl;
    }
    return cmd;
}


// Server::Command Server::parseRawLine(const std::string &line) {
	// Command cmd;
	// size_t pos = 0;
	// size_t len = line.size();

	// // 1. Prefix (optional)
	// if (pos < len && line[pos] == ':') {
		// size_t spacePos = line.find(' ', pos);
		// if (spacePos == std::string::npos) {
			// return cmd; // malformed line
		// }
		// cmd.prefix = line.substr(pos + 1, spacePos - pos - 1);
		// pos = spacePos + 1;
	// }
	// std::cout << "this is the prefix = " << cmd.prefix << std::endl;
	// // Skip spaces
	// while (pos < len && line[pos] == ' ') pos++;

	// // 2. Command (required)
	// size_t spacePos = line.find(' ', pos);
	// if (spacePos == std::string::npos) {
		// cmd.command = line.substr(pos);
	// } else {
		// cmd.command = line.substr(pos, spacePos - pos);
		// pos = spacePos + 1;
	// }
	// // Uppercase command
	// for (size_t i = 0; i < cmd.command.size(); i++) {
		// cmd.command[i] = std::toupper(cmd.command[i]);
	// }
	// std::cout << "this is the command = " << cmd.command << std::endl;
	// // Skip spaces
	
	// while (pos < len && line[pos] == ' ') pos++;
	// if (pos >= len || pos == 0){
		// cmd.params.clear();
		// return cmd;
	// }
	// // 3. params
	// while (pos < len) {
		// std::cout << "working on the params" << pos << std::endl;
		// if (line[pos] == ':') {
			// // Trailing param: take rest of line
			// cmd.params.push_back(line.substr(pos + 1));
			// break;
		// }

		// size_t nextSpace = line.find(' ', pos);
		// if (nextSpace == std::string::npos) {
			// cmd.params.push_back(line.substr(pos));
			// break;
		// } else {
			// cmd.params.push_back(line.substr(pos, nextSpace - pos));
			// pos = nextSpace + 1;
			// while (pos < len && line[pos] == ' ') pos++;
		// }
	// }
	// for (size_t i = 0; i < cmd.params.size(); i++) {
		// std::cout << "this is the param[" << i << "] = " << cmd.params[i] << std::endl;
	// }

	// return cmd;
// }




void Server::handleClientWrite(int fd)
{
	/*****************************************************************************/
	/* Function: handleClientWrite                                               */
	/* How it works:                                                             */
	/*   1. Check if client's _outbuf is empty -> stop POLLOUT monitoring.       */
	/*   2. Send data from _outbuf using send().                                 */
	/*   3. Remove the sent portion from _outbuf.                                */
	/* Key points:                                                               */
	/*   - Handle EAGAIN/EWOULDBLOCK/EINTR errors by retrying later.             */
	/*   - Stop POLLOUT when _outbuf becomes empty.                              */
	/*****************************************************************************/

	// Find the client by fd
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it == _fd_to_client.end())
		return; // if no more client return

	Client* client = it->second;
	std::string &out = const_cast<std::string&>(client->getOutbuf());

	// If Nothing to send → stop listening for POLLOUT
	if (out.empty()) 
	{
		disableWriteInterest(fd);
		client->setWantsWrite(false);
		return;
	}

	// Try to send from the beginning of _outbuf
	#ifdef MSG_NOSIGNAL
		ssize_t n = send(fd, out.data(), out.size(), MSG_NOSIGNAL);
	#else
		ssize_t n = send(fd, out.data(), out.size(), 0);
	#endif

	if (n > 0) 
	{
		// Remove the part we just sent
		out.erase(0, static_cast<size_t>(n));

		if (out.empty()) 
		{
			// When everything is sent stop POLLOUT
			disableWriteInterest(fd);
			client->setWantsWrite(false);
		}
		return;
	}

	// Error handling
	if (n == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
		{
			// Not a fatal error, just try again later
			client->setWantsWrite(true);
			return;
		}
		
		if (errno == EPIPE || errno == ECONNRESET)
		{
			// Client disconnected
			removeClient(fd);
			return;
		}

		// Unexpected error, log + cleanup
		std::cerr << "send() error on fd " << fd << ": " << strerror(errno) << std::endl;
		removeClient(fd);
		return;
	}

	// PS: n == 0 is rare for send(), treat like "nothing sent"
}

void Server::removeClient(int fd)
{
	/*****************************************************************************/
	/* Function: removeClient                                                    */
	/* How it works:                                                             */
	/*   1. Close the client socket.                                             */
	/*   2. Remove fd from _poll_fds.                                            */
	/*   3. Delete Client object and remove from _fd_to_client.                  */
	/*   4. Remove from _nick_to_client map if client was registered.            */
	/* Key points:                                                               */
	/*   - Prevent memory leaks by deleting Client objects.                      */
	/*   - Ensure fd is no longer monitored in _poll_fds.                        */
	/*****************************************************************************/

	std::cout << "Removing client fd=" << fd << std::endl;

	// 1. Close the socket
	close(fd);

	// 2. Remove from poll fds
	int idx = findPollIndex(fd);
	if (idx != -1)
		_poll_fds.erase(_poll_fds.begin() + idx);

	// 3. Remove from fd_to_client and delete client object
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it != _fd_to_client.end())
	{
		delete it->second;
		_fd_to_client.erase(it);
	}

	// 4. Remove from nick_to_client
	for (std::map<std::string, Client*>::iterator nit = _nick_to_client.begin(); nit != _nick_to_client.end(); ++nit)
	{
		if (nit->second->getFd() == fd)
		{
			_nick_to_client.erase(nit);
			break;
		}
	}
	std::cout << "Client fd=" << fd << " removed successfully" << std::endl;
}

void Server::sendToFd(int fd, const std::string& msg) { (void)fd; (void)msg; /* TO DO; */ }
