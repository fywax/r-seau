#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "Requete.h"
#include "data.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void AfficheRequete(FILE *fp, struct Requete R)
{
    fprintf(fp, "> Type de requete : %d\n", R.Type);
    fprintf(fp, "> Numero : %d\n", R.Numero);
    fprintf(fp, "> Numero de la facture : %d\n", R.NumeroFacture);
    fprintf(fp, "> Date : %ld\n", R.Date);
    fprintf(fp, "> Reference : %d\n", R.Reference);
    fprintf(fp, "> Quantite : %d\n", R.Quantite);
    fprintf(fp, "> Prix : %d\n", R.Prix);
    fprintf(fp, "> Constructeur : %s\n", R.Constructeur);
    fprintf(fp, "> Modele : %s\n", R.Modele);
    fprintf(fp, "> NomClient : %s\n", R.NomClient);
    fprintf(fp, "> Type motorisation : %s\n", R.Motorisation);
}

void HandleResponse(struct Requete R)
{
    if (R.Type != OK)
    {
        printf("(Error) The server sent a negative response.\n");
        printf("Either your request wasn't correct, or the server failed.\n");
    }
    else
    {
        fprintf(stderr, "(Success) The following packet has been received : \n");
        AfficheRequete(stderr, R);
        fprintf(stderr, "\nConstructeur : %s, Modele : %s\n", R.Constructeur, R.Modele);
    }
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    int bytesSent, bytesRcvd, totalBytesRcvd; /* Bytes read in single recv() and total bytes read */
    char bufferString[80];
    struct Requete ARequest;
    int userChoice;

    if (argc != 3) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Port>\n", argv[0]);
        exit(1);
    }

    /* Set variables */
    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Second arg: server Port */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("(Error) socket() failed\n");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("(Error) connect() failed\n");

    do
    {
        printf("1) Faire une Recherche\n");
        printf("2) Exit\n");
        printf("---------------------------\n");
        printf("Votre choix: ");
        scanf("%d", &userChoice);

        switch (userChoice)
        {
            case 1: /* Faire une Recherche */
                printf("Entrez le numero de reference: ");
                scanf("%s", bufferString);
                ARequest.Type = Question;
                ARequest.Reference = atoi(bufferString);

                /* Send the structure to the server */
                if ((bytesSent = write(sock, &ARequest, sizeof(struct Requete))) != sizeof(struct Requete))
                    DieWithError("(Error) send() sent a different number of bytes than expected\n");
                fprintf(stderr, "(Success) %d Bytes sent\n", bytesSent);

                /* Receive the same structure back from the server */
                totalBytesRcvd = 0;
                while (totalBytesRcvd < sizeof(struct Requete))
                {
                    if ((bytesRcvd = read(sock, &ARequest, sizeof(struct Requete))) <= 0)
                        DieWithError("(Error) recv() failed or connection closed prematurely\n");
                    fprintf(stderr, "(Success) Packet of %d Bytes were received\n", bytesRcvd);
                    totalBytesRcvd += bytesRcvd;
                }

                /* Handle the response */
                HandleResponse(ARequest);
                break;

            case 2: /* Exit */
                printf("Fermeture du client.\n");
                break;

            default:
                printf("(Error) Option invalide. Veuillez réessayer.\n\n");
                break;
        }

    } while (userChoice != 2);

    close(sock);
    exit(0);
}
