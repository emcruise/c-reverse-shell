#!/usr/bin/env python3
import sys
import socket


def parse_args():
    if len(sys.argv) != 3:
        print('./server.py <host> <port>')
        sys.exit(1)
    return str(sys.argv[1]), int(sys.argv[2])

def create_socket(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((host, port))
    s.listen(1)
    print('Listening for connection on {}:{}'.format(host, port))
    return s.accept()

def loop(conn, addr):
    while True:
        try:
            cmd = input('{}:{} > '.format(addr[0], addr[1]))
        except KeyboardInterrupt:
            print()
            conn.close()
            sys.exit(0)
        cmd = cmd + '\x00'
        conn.send(cmd.encode('utf-8'))
        resp = conn.recv(2048).decode('utf-8')
        print(resp)

if __name__ == '__main__':
    h, p = parse_args()
    c, a = create_socket(h, p)
    loop(c, a)
