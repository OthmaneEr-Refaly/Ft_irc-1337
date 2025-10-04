#!/usr/bin/env python3
import socket
import time
import select

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 44554
PASSWORD = "montassir"
CHANNEL = "#room1"
TOTAL_CLIENTS = 20   # reduce if testing first, increase later to 100

clients = {}
nicknames = {}

def send_line(sock, line):
    sock.sendall((line + "\r\n").encode())
    print(f"[SEND:{nicknames.get(sock,'?')}] {line}")

def make_client(nick):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setblocking(False)   # non-blocking mode
    s.connect_ex((SERVER_HOST, SERVER_PORT))  # connect_ex avoids blocking
    nicknames[s] = nick
    send_line(s, f"PASS {PASSWORD}")
    send_line(s, f"NICK {nick}")
    send_line(s, f"USER {nick} 0 * :{nick} Realname")
    return s

def read_from_clients(sockets):
    """Read all available messages and print them"""
    readable, _, _ = select.select(sockets, [], [], 0.1)
    for s in readable:
        try:
            data = s.recv(4096)
            if not data:
                continue
            lines = data.decode(errors="ignore").split("\r\n")
            for line in lines:
                if line.strip():
                    print(f"[RECV:{nicknames.get(s,'?')}] {line}")
        except BlockingIOError:
            continue
        except Exception as e:
            print(f"[ERROR:{nicknames.get(s,'?')}] {e}")

def main():
    # === Operator Client (Alice) ===
    alice = make_client("Alice")
    clients["Alice"] = alice
    time.sleep(0.1)

    send_line(alice, f"JOIN {CHANNEL}")
    time.sleep(0.2)

    # === Spawn Clients ===
    for i in range(1, TOTAL_CLIENTS + 1):
        nick = f"User{i}"
        s = make_client(nick)
        clients[nick] = s
        # Alice invites each client
        send_line(alice, f"INVITE {nick} {CHANNEL}")
        time.sleep(0.05)
        # client joins
        send_line(s, f"JOIN {CHANNEL}")
        time.sleep(0.02)

    # === Each client sends PRIVMSG ===
    for i in range(1, TOTAL_CLIENTS + 1):
        nick = f"User{i}"
        s = clients[nick]
        send_line(s, f"PRIVMSG {CHANNEL} :Hello from {nick}")
        time.sleep(0.01)

    # Read server replies
    read_from_clients(list(clients.values()))

    # === Alice kicks everyone ===
    for i in range(1, TOTAL_CLIENTS + 1):
        nick = f"User{i}"
        send_line(alice, f"KICK {CHANNEL} {nick} :Goodbye {nick}")
        time.sleep(0.02)

    # Read server replies
    read_from_clients(list(clients.values()))

    # Clean up
    for s in list(clients.values()):
        try:
            send_line(s, "QUIT :Bye")
            s.close()
        except:
            pass

if __name__ == "__main__":
    main()