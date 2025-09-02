#ifndef HEADERS_HPP
#define HEADERS_HPP

// ===== C++ standard headers =====
#include <iostream>      // std::cout, std::cerr
#include <string>        // std::string
#include <vector>        // std::vector
#include <map>           // std::map
#include <cstring>       // std::strerror, std::memset

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

#endif
