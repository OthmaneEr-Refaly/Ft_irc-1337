#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>
#include <vector>

class Client
{
	private:
    // ===== Network state =====
		int			_fd;			// Socket file descriptor for this client
		std::string	_inbuf;			// Data received but not yet processed
		std::string _outbuf;		// Data to send when socket is writable
		bool		_closing;// True if the connection should be closed

	// ===== Identity =====
		bool		_registered; 	// True if PASS, NICK, USER completed
		bool		_passOk;		// True if PASS is set correctly
		std::string	_nick; 			// NICK command value
		std::string	_user; 			// USER command value
		std::string	_realname; 		// USER command value real name field
		std::string	_host; 			// Client's hostname/IP

    // ===== Channel membership =====
		std::set<std::string> _channels;

	
	public:
	// ===== Canonical form =====
				Client();
				Client(const Client& other);
		Client&	operator=(const Client& other);
				~Client();
				Client(int fd);

	// ===== Getters =====
		int								getFd() const;
		const std::string&				getInbuf() const;
		const std::string&				getOutbuf() const;
		std::string&					getOutbufRef(); // allows safe modification of outbuf
		bool 							isRegistered() const;
		bool 							isClosing() const;
		bool 							isPassOk() const;
		const std::string&				getNick() const;
		const std::string&				getUser() const;
		const std::string&				getRealname() const;
		const std::string&				getHost() const;
		const std::set<std::string>&	getChannels() const;

	// ===== Setters =====
		void setFd(int fd);
		void setInbuf(const std::string& buf);
		void setOutbuf(const std::string& buf);
		void setRegistered(bool value);
		void setPassOk(bool value);
		void setNick(const std::string& nick);
		void setUser(const std::string& user);
		void setRealname(const std::string& realname);
		void setHost(const std::string& host);
		void setChannels(const std::set<std::string>& channels);

	// ===== Buffer Helpers =====
		void 						appendToInbuf(const std::string& data);		// Add received data
		void						enqueueOutput(const std::string& msg);		// Queue data for sending
		std::vector<std::string>	popCompleteLines();							// Extract full IRC lines

	// ===== Channel helpers =====
		void addChannel(const std::string& channel);
		void removeChannel(const std::string& channel);
	
	// ===== Connection control =====
		void markForClose();	// Prepare client for removal
	
	// ===== Messaging =====
		void sendNumericReply(Server &server, int code, const std::string &arg, const std::string &message);
};

#endif
