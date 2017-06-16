import socket, sys, signal

HOST = '127.0.0.1'     # Server IP address
PORT = 5000            # Server port

#Exit handler
def signal_handler(signal, frame):
    tcp_client.close()
    print "\nEncerrado. Obrigado por usar!"
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

#Init socket's variables
tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)

#Connect to server
tcp_client.connect(dest)
print 'Para sair, use CTRL+C.\n'

while True:
	print "Insira o escalar a multiplicar:"
	param=raw_input()
	print "Insira o numero de linhas da matriz:"
	h=input()
	param+=" "+str(h)
	print "Insira o numero de colunas da matriz:"
	w=input()
	param+=" "+str(w)
	for i in range(h):
		print "Escreva a linha "+str(i+1)+" da matriz"
		param+=" "+raw_input()
	tcp_client.send (param)
	answer = tcp_client.recv(1024)
	print "\n"+answer

#msg = "3 3 3 1 2 3 4 5 6 7 8 9"
#tcp_client.send (msg)
#answer = tcp_client.recv(1024)
#print answer
#tcp_client.close()
