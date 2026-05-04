# FT_IRC - C++ IRC Server

![IRC Server Logo/Banner]([https://via.placeholder.com/800x200?text=FT_IRC+Server+Banner](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9))

## Brief Overview
This project is a custom implementation of an Internet Relay Chat (IRC) server written in C++98. It is designed to be fully compliant with standard IRC clients (such as Irssi or Netcat) allowing users to connect, communicate, and manage channels. The server handles multiplexing using `poll()` for non-blocking I/O operations and enables multiple clients to chat and interact concurrently. 

Key features include:
- Client authentication (`PASS`, `NICK`, `USER`).
- Channel management and operations (`JOIN`, `PART`, `TOPIC`, `KICK`, `INVITE`, `MODE`).
- Seamless non-blocking I/O multiplexing with `poll()`.
- Built-in Bot behavior implementation.

## What I Learned
During the development of this project, specifically while focusing on server initialization and IRC commands, I gained valuable insights into:
- **Network Programming:** Working efficiently with sockets, binding, listening, and managing client connections.
- **I/O Multiplexing:** Mastering the `poll()` function to handle multiple concurrent clients over a single-threaded architecture, ensuring smooth non-blocking operations.
- **Protocol Implementation:** Gaining a deep understanding of the IRC protocol, parsing line-based communication buffers, and properly formatting responses and numeric replies according to standards.
- **String Parsing and Command Dispatching:** Implementing robust algorithms for parsing incoming network buffers, handling partial reads/writes, and accurately mapping incoming text commands to their respective C++ handler functions.
- **C++98 Standard:** Adhering strictly to C++98 limits, improving my skills in object-oriented principles, memory management, and employing STL containers to manage state (like clients and channels).

## How to Use

### Prerequisites
- A UNIX-like environment (Linux/macOS)
- `c++` compiler (e.g., `g++` or `clang++`)
- `make`

### Building the Server
Run the following command in the root directory to compile the project:
```bash
make
```
This will produce an executable named `ircserv`.

### Running the Server
The executable requires two arguments: a listening `port` and a `password` that clients must use to authenticate.
```bash
./ircserv <port> <password>
```
**Example:**
```bash
./ircserv 6667 my_secure_password
```

### Connecting as a Client
You can test the server using popular IRC clients such as `irssi` or standard tools like Netcat.

**Using Netcat (`nc`):**
```bash
nc 127.0.0.1 6667
PASS my_secure_password
NICK mynick
USER myuser 0 * :My Real Name
```

**Using Irssi:**
```bash
irssi -c 127.0.0.1 -p 6667 -w my_secure_password -n mynick
```

### Supported Commands
Once connected, you can interact with the server using standard IRC commands:
- `/JOIN <channel>` - Join or create a channel
- `/PRIVMSG <target> <message>` - Send a private message to a specific user or a channel
- `/KICK <channel> <user>` - Kick a user from a channel
- `/INVITE <user> <channel>` - Invite a user to a channel
- `/TOPIC <channel> [<topic>]` - View or set the channel's topic
- `/MODE <channel> <+|-mode> [params]` - Modify channel modes (e.g., `i` for invite-only, `t` for topic restriction, `k` for passwords, `o` for operators, `l` for limits)
- `/PART <channel>` - Leave a channel
- `/QUIT` - Disconnect from the server
