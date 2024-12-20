#ifndef REQUETE_H
#define REQUETE_H

#include <stdio.h>

enum TypeRequete
{
    Question = 1,
    Achat = 2,
    Livraison = 3,
    FactureHV = 4,  // Ajout de FactureHV ici
    OK = 5,
    Fail = 6
};

struct Requete
{
  enum TypeRequete Type;
  int Numero; // Contient le numéro de la requete
  int NumeroFacture;
  time_t Date;
  int Reference; // La référence du film
  int Quantite;
  int Prix;
  char Constructeur[30];
  char Modele[30];
  char NomClient[80];
  char Motorisation[30];
};

#endif
