#ifndef LIB_SER_HV_H
#define LIB_SER_HV_H

#include "data.h"

void DieWithError(char *errorMessage) ;
int NombreVehiculesHV(const char *NomFichier);
void CreationAjoutVehiculeHV(const char *NomFichier, const struct VehiculeHV *UnRecord);
int RechercheHV(const char *NomFichier, int Reference, struct VehiculeHV *UnRecord);
int ReservationHV(const char *NomFichier, int Reference, int Quantite);
int FacturationHV(const char *NomFichier, const char *NomClient, time_t Date, int Quantite, int Reference);
void ListingVehiculesHV(const char *NomFichier);
void ListingFacturesHV(const char *NomFichier);
void AProposServeurHV(char *Version,char *Nom1,char* Nom2) ;
void MajStockVehiculeHV(int Reference, int Quantite);

#endif
