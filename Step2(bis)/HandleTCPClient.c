#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>
#include "HandleTCPClient.h"
#include "LibSerHV.h"
#include "Requete.h"
#include "data.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void HandleTCPClient(int clntSocket)
{
  int recvMsgSize; /* Size of received message */
  struct Requete ARequest;

  /* Receive message from client */
  if ((recvMsgSize = read(clntSocket, &ARequest, sizeof(struct Requete))) < 0)
      DieWithError("(Error) recv() failed\n");

  /* Send received string and receive again until end of transmission */
  while (recvMsgSize > 0) /* zero indicates end of transmission */
  {
    printf("(Success) Packet of %d Bytes received\n", recvMsgSize);

    switch (ARequest.Type)
    {
      case Question:
        caseQuestion(&ARequest);
        break;

      case Achat:
        printf("(Error) Achat requests aren't supported");
        break;

      case Livraison:
        printf("(Error) Livraison requests aren't supported");
        break;

      default:
        printf("(Error) This type of request isn't supported");
    }

    /* Send the response message back to the client */
    if (write(clntSocket, &ARequest, sizeof(struct Requete)) != sizeof(struct Requete))
        DieWithError("(Error) send() failed\n");

    /* See if there is more data to receive */
    if ((recvMsgSize = read(clntSocket, &ARequest, sizeof(struct Requete))) < 0)
        DieWithError("(Error) recv() failed\n");
  }

  close(clntSocket);    /* Close client socket */
}

void caseQuestion(struct Requete* R)
{
  struct VehiculeHV VehiculeData;

  printf("(Process) Searching for reference %d...\n", R->Reference);
  if (RechercheHV("VehiculesHV", R->Reference, &VehiculeData) == -1)
  {
    printf("(Error) Search failed or didn't find anything\n");
    R->Type = Fail;
  }
  else
  {
    printf("(Success) Reference %d found by research\n", R->Reference);

    printf("(Process) Converting the result to a packet...\n");
    ConvertVehiculeVHToRequest(R, VehiculeData);
  }
}

void ConvertVehiculeVHToRequest(struct Requete* R, struct VehiculeHV V)
{
  R->Type = OK;
  R->Numero = 0;
  R->NumeroFacture = 0;
  R->Reference = V.Reference;
  R->Quantite = V.Quantite;
  R->Prix = 0;
  strcpy(R->Constructeur, V.Constructeur);
  strcpy(R->Modele, V.Modele);
  strcpy(R->NomClient, "Non defini");
  strcpy(R->Motorisation, V.Motorisation);
}
