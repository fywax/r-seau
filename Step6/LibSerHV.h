#ifndef LIBSERHV_H
#define LIBSERHV_H

#include "data.h"

// Prototypes des fonctions
int NombreVehiculesHV(const char *NomFichier);
void CreationAjoutVehiculeHV(const char *NomFichier, const struct VehiculeHV *UnRecord);
int rechercheHV(const char *NomFichier, int Reference, struct VehiculeHV *UnRecord);
int ReservationHV(const char *NomFichier, int Reference, int Quantite);
int FacturationHV(const char *NomFichier, const char *NomClient, time_t Date, int Quantite, int Reference);
void ListingVehiculesHV(const char *NomFichier);
void ListingFacturesHV(const char *NomFichier);
void AProposServeurHV(char *Version,char *Nom1,char* Nom2) ;


#endif
