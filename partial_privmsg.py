#!/usr/bin/env python3
import socket, time

HOST = '127.0.0.1'
PORT = 44554  # change to your server port

s = socket.create_connection((HOST, PORT))
s.settimeout(1.0)

# optionally read server greeting (non-blocking)
try:
    print("server:", s.recv(4096))
except socket.timeout:
    pass

s.sendall(b'pass montassir\r\n')
s.sendall(b'nick y\r\n')
s.sendall(b'user y y y y\r\n')

# send first chunk WITHOUT CRLF
s.sendall(b'PRIVMSG mo :hello, this is half')
print("sent part1")

time.sleep(2)  # wait to simulate fragmentation / pause

# send remainder + CRLF
s.sendall(b'world\r\n')
print("sent part2")

# read server responses
time.sleep(0.5)
try:
    while True:
        data = s.recv(4096)
        if not data:
            break
        print("recv:", data.decode('utf-8', errors='replace'))
except socket.timeout:
    pass

s.close()

