import socket
import sys

HOST = '127.0.0.1'     # Server IP address
PORT = 5000            # Server port

#Init socket's variables
tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)

#Connect to server
tcp_client.connect(dest)
print 'Para sair use CTRL+X\n'

msg = "3 '3' '3' '1' '2' '3' '4' '5' '6' '7' '8' '9'"
tcp_client.send (msg)
answer = tcp_client.recv(1024)
print answer
tcp_client.close()
