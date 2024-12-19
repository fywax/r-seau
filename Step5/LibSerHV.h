#ifndef LIB_SER_HV_H
#define LIB_SER_HV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "data.h"

void DieWithError(char *errorMessage) ;
void AProposServeurHV(char *Version,char *Nom1,char* Nom2) ;
int RechercheHV(char* NomFichier, int Reference, struct VehiculeHV *UnRecord);

#endif
