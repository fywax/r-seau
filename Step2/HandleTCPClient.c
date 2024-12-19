#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include "Requete.h"
#include "LibSerHV.h"   /* For RechercheHV and VehiculeHV structure */

#define RCVBUFSIZE 32   /* Size of receive buffer */



void HandleTCPClient(int clntSocket) {
    int recvMsgSize;                      /* Size of received message */
    struct Requete UneRequete;            /* Request structure */
    struct VehiculeHV *VehiculeTrouve;    /* Pointer to found vehicle */

    /* Receive request from client */
    if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof(struct Requete), 0)) < 0) {
        DieWithError("recv() failed");
    }

    while (recvMsgSize > 0) {
        printf("Requête reçue avec Numero : %d\n", UneRequete.Numero);

        /* Call RechercheHV to find the vehicle based on Numero */
        VehiculeTrouve = rechercheHV("NomFichier", UneRequete.Reference, UneRequete.Numero);
        if (VehiculeTrouve != NULL) {
            printf("Véhicule trouvé : %s, %s, Année %d\n",
                   VehiculeTrouve->Marque,
                   VehiculeTrouve->Modele,
                   VehiculeTrouve->Annee);

            /* Send vehicle information back to the client */
            if (write(clntSocket, VehiculeTrouve, sizeof(struct VehiculeHV)) != sizeof(struct VehiculeHV)) {
                DieWithError("send() failed");
            }
        } else {
            printf("Aucun véhicule trouvé pour Numero : %d\n", UneRequete.Numero);

            /* Send an empty response to indicate no vehicle found */
            struct VehiculeHV Vide = {0}; /* Initialize all fields to zero */
            if (write(clntSocket, &Vide, sizeof(struct VehiculeHV)) != sizeof(struct VehiculeHV)) {
                DieWithError("send() failed");
            }
        }

        /* Check if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, &UneRequete, sizeof(struct Requete), 0)) < 0) {
            DieWithError("recv() failed");
        }
    }

    printf("Connexion fermée.\n");
    close(clntSocket); /* Close client socket */
}
