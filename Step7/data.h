#ifndef DATA_H
#define DATA_H

#include <time.h>
#include "Requete.h"  // Assurez-vous d'inclure requete.h

struct VehiculeHV
{
    int Supprime;
    int Reference;
    char Constructeur[30];
    char Modele[30];
    int Quantite;
    char Motorisation[30];
};

struct FactureHV
{
    int NumeroFacturation;
    char Acheteur[40];
    time_t DateFacturation;
    int Quantite;
    int Reference;
};

// Vous pouvez maintenant utiliser les types de TypeRequete directement
typedef enum TypeRequete TypeDemande;

#endif
