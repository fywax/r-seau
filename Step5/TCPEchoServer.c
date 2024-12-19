#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>   /* pour getaddrinfo() et struct addrinfo */
#include "LibSerHV.h"
#include "HandleTCPClient.h"
#include "Requete.h"

#define MAXPENDING 5

int main(int argc, char *argv[])
{
    int servSock;
    int clntSock;
    struct addrinfo hints, *servInfo, *p;
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;
    int clntLen;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Server IP Address> <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[2]);

    // Initialisation de hints pour getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // Socket stream TCP
    hints.ai_flags = AI_PASSIVE;        // Utilisation de l'adresse locale

    if (getaddrinfo(argv[1], argv[2], &hints, &servInfo) != 0)
    {
        perror("getaddrinfo failed");
        exit(1);
    }

    // Création du socket
    if ((servSock = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) < 0)
        DieWithError("(Error) socket() failed\n");
    else
        printf("(Success) socket() OK\n");

    // Bind avec la structure retournée par getaddrinfo
    if (bind(servSock, servInfo->ai_addr, servInfo->ai_addrlen) < 0)
    {
        DieWithError("(Error) bind() failed\n");
    }
    else
    {
        printf("(Success) bind() OK\n");
    }

    freeaddrinfo(servInfo);  // Libération de la mémoire allouée par getaddrinfo

    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("(Error) listen() failed\n");
    else
        printf("(Success) listen() OK\n");

    while (1)
    {
        clntLen = sizeof(echoClntAddr);
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("(Error) accept() failed\n");
        else
            printf("(Success) accept() OK\n");

        printf("(Process) Handling client %s...\n", inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock);
    }

    close(servSock);
    return 0;
}
