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
	bool		_want_write;	// True if poll() should watch for POLLOUT

	// ===== Identity =====
	bool		_registered; 	// True if PASS, NICK, USER completed
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
	bool							getWantsWrite() const;
	bool 							isRegistered() const;
	const std::string&				getNick() const;
    const std::string&				getUser() const;
    const std::string&				getRealname() const;
    const std::string&				getHost() const;
    const std::set<std::string>&	getChannels() const;

	// ===== Setters =====
	void setFd(int fd);
    void setInbuf(const std::string& buf);
    void setOutbuf(const std::string& buf);
    void setWantsWrite(bool value);
    void setRegistered(bool value);
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
};

#endif