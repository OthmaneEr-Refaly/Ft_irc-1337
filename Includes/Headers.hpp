#ifndef HEADERS_HPP
#define HEADERS_HPP

#define IRC_MAX_LINE 512
#define IRC_MAX_CONTENT (IRC_MAX_LINE - 2)

// ===== Local / Project headers =====
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"
#include "NumericReplies.hpp"

// ===== C++ standard headers =====
#include <iostream>      // std::cout, std::cerr
#include <string>        // std::string
#include <vector>        // std::vector
#include <map>           // std::map
#include <set>           // std::set
#include <cstring>       // std::strerror, std::memset
#include <sstream>		 //	std::stringstream

// ===== POSIX / system headers =====
#include <sys/types.h>   // basic system types (socklen_t, etc.)
#include <sys/socket.h>  // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h>  // sockaddr_in, htons, INADDR_ANY
#include <arpa/inet.h>   // inet_ntoa, ntohs, etc. (if needed)
#include <unistd.h>      // close()
#include <fcntl.h>       // fcntl()
#include <poll.h>        // poll(), struct pollfd
#include <errno.h>       // errno
#include <stdio.h>       // perror()
#include <stdlib.h>		 // atoi()

#endif