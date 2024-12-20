#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>  /* pour getaddrinfo() et struct addrinfo */
#include "Requete.h"
#include "data.h"
#include "LibSerHV.h"

// include pour les fonctions entrees sortie
#include <termios.h>
#include <unistd.h>

#define RCVBUFSIZE 32

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
    int sock;
    struct sockaddr_in;
    struct addrinfo hints, *servInfo;
    int bytesSent, bytesRcvd, totalBytesRcvd;
    char bufferString[80];
    struct Requete ARequest;
    int userChoice, ref;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Port>\n", argv[0]);
        exit(1);
    }

    // Initialisation de hints pour getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;         // IPv4
    hints.ai_socktype = SOCK_STREAM;   // Socket stream TCP

    if (getaddrinfo(argv[1], argv[2], &hints, &servInfo) != 0)
    {
        perror("getaddrinfo failed");
        exit(1);
    }

    // Création du socket
    if ((sock = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) < 0)
        DieWithError("(Error) socket() failed\n");

    // Connexion au serveur
    if (connect(sock, servInfo->ai_addr, servInfo->ai_addrlen) < 0)
        DieWithError("(Error) connect() failed\n");

    freeaddrinfo(servInfo);  // Libération de la mémoire allouée par getaddrinfo

    do
    {
        printf("1) Faire une Recherche\n");
        printf("2) Faire une Facturation\n"); // Nouvelle option pour la facturation
        printf("3) Exit\n");
        printf("---------------------------\n");
        printf("Votre choix: ");
        scanf("%d", &userChoice);

        switch (userChoice)
        {
        case 1:
            printf("Entrez le numero de reference: ");
            scanf("%d", &ref);
            struct VehiculeHV UnRecord;
            RechercheHV("VehiculesHV", ref, &UnRecord);
            ARequest.Type = Question;

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

        case 2:
            // Gestion de la facturation
            printf("Entrez le Nom du client: ");
            scanf("%s", ARequest.NomClient);
            printf("Entrez le numero de reference: ");
            scanf("%d", &ARequest.Reference);
            printf("Entrez la quantité commandée: ");
            scanf("%d", &ARequest.Quantite);

            ARequest.Type = Achat;  // Type de requête pour la facturation

            /* Send the structure to the server */
            if ((bytesSent = write(sock, &ARequest, sizeof(struct Requete))) != sizeof(struct Requete))
                DieWithError("(Error) send() sent a different number of bytes than expected\n");
            fprintf(stderr, "(Success) %d Bytes sent for Facturation\n", bytesSent);

            /* Receive the same structure back from the server */
            totalBytesRcvd = 0;
            while (totalBytesRcvd < sizeof(struct Requete))
            {
                if ((bytesRcvd = read(sock, &ARequest, sizeof(struct Requete))) <= 0)
                    DieWithError("(Error) recv() failed or connection closed prematurely\n");
                fprintf(stderr, "(Success) Packet of %d Bytes were received\n", bytesRcvd);
                totalBytesRcvd += bytesRcvd;
            }

            MajStockVehiculeHV(ARequest.Reference, ARequest.Quantite);
            FacturationHV("FacturesHV", ARequest.NomClient, time(NULL),  ARequest.Quantite,  ARequest.Reference);

            /* Affiche les détails de la facture reçue */
            printf("\n--- Facture ---\n");
            printf("Numero de facture: %d\n", ARequest.NumeroFacture);
            printf("Nom du client: %s\n", ARequest.NomClient);
            printf("Reference: %d\n", ARequest.Reference);
            printf("Quantite: %d\n", ARequest.Quantite);

            // Vous pouvez ajouter d'autres détails si nécessaire, par exemple le prix ou la date de facturation
            break;

        case 3:
            printf("Fermeture du client.\n");
            break;

        default:
            printf("(Error) Option invalide. Veuillez réessayer.\n\n");
            break;
        }

    } while (userChoice != 3);

    close(sock);
    exit(0);
}
