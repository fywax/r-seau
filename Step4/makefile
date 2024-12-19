CFLAGS = -Wall -g
OBJS = LibSerHV.o HandleTCPClient.o

all: TCPEchoClient TCPEchoServer

TCPEchoClient: TCPEchoClient.c Requete.h data.h
	gcc $(CFLAGS) -o TCPEchoClient TCPEchoClient.c

TCPEchoServer: TCPEchoServer.c HandleTCPClient.o LibSerHV.o Requete.h data.h
	gcc $(CFLAGS) -o TCPEchoServer TCPEchoServer.c HandleTCPClient.o LibSerHV.o

LibSerHV.o: LibSerHV.c LibSerHV.h data.h Requete.h
	gcc $(CFLAGS) -c -o LibSerHV.o LibSerHV.c

HandleTCPClient.o: HandleTCPClient.c HandleTCPClient.h LibSerHV.h data.h Requete.h
	gcc $(CFLAGS) -c -o HandleTCPClient.o HandleTCPClient.c
