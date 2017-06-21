import os, subprocess, socket, signal, sys

HOST = ''              # Server IP address
PORT = 5000            # Server port

#Exit handler
def signal_handler(signal, frame):
    print "\nServidor finalizado!"
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

#Init socket's variables
tcp_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = (HOST, PORT)

#Bind socket
tcp_server.bind(server_address)

#Listen clients
tcp_server.listen(5)

print "Servidor iniciado!"

#Loop while to connect clients and message delivery
while True:
    con, cliente = tcp_server.accept()
    print 'Conectado a', cliente
    while True:
        msg = con.recv(1024)
        if not msg: break
        con.send(subprocess.check_output("./main_g7 "+msg, shell=True))
    print 'Finalizando conexao do cliente', cliente
    con.close()
