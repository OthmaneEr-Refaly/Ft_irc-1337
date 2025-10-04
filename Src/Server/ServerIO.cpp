/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:37:51 by mobouifr          #+#    #+#             */
/*   Updated: 2025/10/04 16:48:43 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/Headers.hpp"

void Server::acceptNewClient()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	int client_fd = accept(_listen_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (client_fd == -1)
	{
		perror("accept");
		return;
	}

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // --- Get client IP/Host ---
    char host[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(client_addr.sin_addr), host, sizeof(host)) == NULL)
    {
        perror("inet_ntop");
        strcpy(host, "unknown");
    }

	Client* new_client = new Client(client_fd);
	new_client->setHost(host);

	_fd_to_client[client_fd] = new_client;

	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;  
	_pollTable.push_back(pfd);

	std::cout << "New client connected (fd=" << client_fd << ")" << std::endl;
}

void Server::handleClientRead(int fd)
{
	// Find client by fd
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it == _fd_to_client.end())
		return; // client not found, maybe already removed

	Client &client = *(it->second);

	char	buffer[512];  // IRC recommends max line length = 512 bytes
	ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_read <= 0)
	{
		Command quitCmd;
		quitCmd.command = "QUIT";

		if (bytes_read == 0) // Client disconnected (0)
			quitCmd.params.push_back("Client disconnected");
		
		else
			quitCmd.params.push_back("Connection error");

		handleQuit(*this, client, quitCmd);
		return;
	}

	buffer[bytes_read] = '\0';

	std::string data(buffer, bytes_read);
	client.appendToInbuf(data);

	std::vector<std::string> lines = client.popCompleteLines(); // Extract complete lines (commands ending with "\r\n")
	std::cout << lines.size() << std::endl;
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::string line = lines[i];

		if (line.size() > IRC_MAX_CONTENT)
		{
			line = line.substr(0, IRC_MAX_CONTENT);
			client.sendNumericReply(*this, ERR_INPUTTOOLONG, "*", "Input line too long, truncated");
		}

		std::cout << "Parsed command from fd " << fd << ": [" << line << "]" << std::endl;
		Command cmd = parseRawLine(line);
		dispatchCommand(*this, client, cmd);
	}

	// // ===== TEST: send a reply back =====
	// client.enqueueOutput("Hello from server!\r\n");
	// enableWriteInterest(fd);
}

void Server::handleClientWrite(int fd)
{
	// Find the client by fd
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it == _fd_to_client.end())
		return; // if no more client return

	Client* client = it->second;
	std::string &out = client->getOutbufRef();

	// If Nothing to send → stop listening for POLLOUT
	if (out.empty()) 
	{
		disableWriteInterest(fd);
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
		}
		return;
	}

	// Error handling
	if (n == -1)
	{
		// Unexpected error, log + cleanup
		std::cerr << "send() error on fd " << fd << ": " << strerror(errno) << std::endl;
		disconnectClient(fd, "Send error");
		return;
	}

	// PS: n == 0 is rare for send(), treat like "nothing sent"
}

void Server::removeClient(int fd)
{

	std::cout << "Removing client fd=" << fd << std::endl;

	// 1. Close the socket
	close(fd);

	// 2. Remove from poll fds
	int idx = findPollIndex(fd);
	if (idx != -1)
		_pollTable.erase(_pollTable.begin() + idx);

	// 3. Remove from fd_to_client and cleanup channels before delete
	std::map<int, Client*>::iterator it = _fd_to_client.find(fd);
	if (it != _fd_to_client.end())
	{
		Client* c = it->second;

		std::string nick = c->getNick();

		const std::set<std::string>& chans = c->getChannels();
		for (std::set<std::string>::const_iterator chit = chans.begin(); chit != chans.end(); ++chit)
		{
			Channel* chan = getChannel(*chit);
			if (chan)
			{
				if (chan->isMember(c))
					chan->removeMember(c);
				if (chan->isOperator(c))
					chan->removeOperator(c);

				chan->removeInvite(c->getNick());

				if (chan->getMembers().empty())
					removeChannel(*chit);
			}
		}

        if (!nick.empty())
            _nick_to_client.erase(nick);

		delete c;
		_fd_to_client.erase(it);
	}

	std::cout << "Client fd=" << fd << " removed successfully" << std::endl;
}

