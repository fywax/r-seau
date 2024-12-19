#ifndef HANDLE_TCP_CLIENT_H
#define HANDLE_TCP_CLIENT_H

#include "Requete.h"
#include "data.h"

void HandleTCPClient(int clntSocket);
void caseQuestion(struct Requete* R);
void ConvertVehiculeVHToRequest(struct Requete* R, struct VehiculeHV V);

#endif
