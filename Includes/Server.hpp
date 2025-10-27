#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>


class Client;
class Channel;
class Bot;

struct Command 
{
	std::string 				prefix;
	std::string					command;
	std::vector<std::string>	params;
};

class Server 
{
	private:
	// ===== Configuration =====
	int				_port;
	std::string		_password;

	
	// ===== Networking state =====
	int							_listen_fd; // Listening socket fd
	std::vector<struct pollfd>	_pollTable;  // Poll list
	bool						_running;   // Main loop flag
	std::string					_creation_date;
	
	// ===== Data structures =====
	std::map<int, Client*>				_fd_to_client;
	std::map<std::string, Client*>		_nick_to_client;
	std::map<std::string, Channel*>		_channels;
	
	// // ===== Bot =====
	Bot* 			_bot;
	
	// ===== Internal helpers (defined in .cpp) =====
	void	initListenSocket();
	void	acceptNewClient();
	void	removeClient(int fd);
	void	handlePollEvents();
	void	processClientInput(int fd);
	void	sendClientResponse(int fd);
	
	// ===== New Poll helper functions =====
	int		findPollIndex(int fd) const;
	void	enableWriteInterest(int fd);
	void	disableWriteInterest(int fd);
	
	
	public:
	// ===== Canonical form =====
	Server();
	Server(int port, const std::string& password);
	Server(const Server& other);
	Server&	operator=(const Server& other);
	~Server();
	
	// ===== Getters =====
	int										getPort() const;
	const std::string&						getPassword() const;
	const std::string&						getCreationDate() const;
	bool									isRunning() const;
	const std::map<int, Client*>&			getFdToClient() const;
	const std::map<std::string, Client*>&	getNickToClient() const;
	const std::map<std::string, Channel*>&	getChannels() const;
	
	// ===== Setters =====
	void 		setRunning(bool value);
	void		setCreationDate();
	
	// ===== Main control =====
	void 		run();  // Start the server loop
	void 		stop(); // Stop the server gracefully
	
	// ===== Parsing functions ====
	Command 	parseRawLine(const std::string &line);
	
	// ===== Client management =====
	void		disconnectClient(int fd, const std::string &reason);
	void		tryRegister(Client &client);
	
	// ===== Nickname management =====
	void		registerNickname(const std::string &nick, Client *client);
	void		unregisterNickname(const std::string &nick);
	bool		isNicknameInUse(const std::string &nick) const;
	Client		*findClientByNick(const std::string &nick);
	
	// ===== Channel management =====
	Channel*	getChannel(const std::string& channelName);
	Channel*	createChannel(const std::string& channelName);
	void		removeChannel(const std::string& channelName);
	
	// ===== Messaging functions =====
	void		sendMsgToClient(Client *client, const std::string &msg);
	std::string	enforceMessageLength(const std::string &rawMessage);
	void		sendMsgToChannel(Channel &chan, const std::string &message, Client *sender);
	
	static Server*	g_instance;
	static void	handleSignal(int signum);

	// ===== Bot management =====
	void initBot();
	Bot* getBot() const;
	void connectBot();
};

#endif
