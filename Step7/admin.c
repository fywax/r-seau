/******************************************
  Herman Vanstapel
  2017 Basé sur Fichiers
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "LibSerHV.h"

// include pour les fonctions entrees sortie
#include <termios.h>
#include <unistd.h>

// Prototypes pour l'affichage
void ClrScr(void);
void Gotoxy(int x, int y);
void DelNewLine(char *Chaine);
void MonPrintf(char *tempo, int espace, int taille);
void AfficheEnteteVehiculeHV();
void AfficheVehiculeHV(struct VehiculeHV *UnRecord);
void SaiSieVehiculeHV(int Reference, struct VehiculeHV *UnRecord);
void MajStockVehiculeHV(int Reference, int Quantite);

// clrscr() function definition
void ClrScr(void) { system("clear"); }

void Gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

// Permet de supprimer le cr qui est placé dans la chaîne lors d'un fgets
void DelNewLine(char *Chaine) {
    Chaine[strlen(Chaine) - 1] = 0;
}

void MonPrintf(char *tempo, int espace, int taille) {
    int Count;
    printf("%s", tempo);
    Count = espace - taille;
    while (Count > 0) {
        printf(" ");
        Count--;
    }
}

void AfficheEnteteVehiculeHV() {
    char Tampon[80];
    sprintf(Tampon, "%s", "Ref");
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", "Constructeur");
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", "Modele");
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s\t", "Quantite");
    MonPrintf(Tampon, 6, strlen(Tampon));
    sprintf(Tampon, "%s", "Motorisation");
    MonPrintf(Tampon, 30, strlen(Tampon));
    printf("\n");
}

void AfficheVehiculeHV(struct VehiculeHV *UnRecord) {
    char Tampon[80];
    sprintf(Tampon, "%d", UnRecord->Reference);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Constructeur);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Modele);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%d\t\t", UnRecord->Quantite);
    MonPrintf(Tampon, 6, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Motorisation);
    MonPrintf(Tampon, 30, strlen(Tampon));
    printf("\n");
}

void SaiSieVehiculeHV(int Reference, struct VehiculeHV *UnRecord) {
    char Tampon[80];

    printf("Reference :%d \n", Reference);
    UnRecord->Reference = Reference;

    printf("Saisie Constructeur :");
    fgets(UnRecord->Constructeur, sizeof UnRecord->Constructeur, stdin);

    printf("Saisie Modele :");
    fgets(UnRecord->Modele, sizeof UnRecord->Modele, stdin);

    printf("Saisie Quantite :");
    fgets(Tampon, sizeof Tampon, stdin);
    UnRecord->Quantite = atoi(Tampon);

    printf("Saisie Motorisation :");
    fgets(UnRecord->Motorisation, sizeof UnRecord->Modele, stdin);

    DelNewLine(UnRecord->Constructeur);
    DelNewLine(UnRecord->Modele);
    AfficheEnteteVehiculeHV();
    AfficheVehiculeHV(UnRecord);
    printf("-----------------------\n");
}


// Fonction main corrigée
int main() {
    char Choix;
    int reference, quantite;
    char NomClient[60];

    while (1) {
        printf("-------2022-----------\n");
        printf("1) Ajout              \n");
        printf("2) Vehicule           \n");
        printf("4) Recherche          \n");
        printf("5) Achat              \n");
        printf("6) Factures           \n");
        printf("7) A propos           \n");
        printf("8) Exit               \n");
        printf("----------------------\n");
        printf(">>");
        Choix = getchar();
        getchar(); // Consommer le '\n' après le choix
        printf("\n");

        switch (Choix) {
            case '1': {
                struct VehiculeHV UnRecord;
                char Redo = 'y';

                while (Redo == 'Y' || Redo == 'y') {
                    int Nombre = NombreVehiculesHV("VehiculesHV");
                    SaiSieVehiculeHV(Nombre + 1, &UnRecord);
                    CreationAjoutVehiculeHV("VehiculesHV", &UnRecord);
                    printf("Encoder un autre (Y/N) ? ");
                    Redo = getchar();
                    getchar(); // Consommer le '\n'
                }
                break;
            }
            case '2':
                ListingVehiculesHV("VehiculesHV");
                break;

            case '4':
                printf("Reference : ");
                scanf("%d", &reference);
                getchar(); // Consommer le '\n'
                struct VehiculeHV UnRecord;
                if (RechercheHV("VehiculesHV", reference, &UnRecord) == 1) {
                    AfficheEnteteVehiculeHV();
                    AfficheVehiculeHV(&UnRecord);
                } else {
                    printf("Vehicule introuvable.\n");
                }
                break;

case '5': {
    // Demande des informations nécessaires pour la facturation
    printf("Nom du client : ");
    fgets(NomClient, sizeof(NomClient), stdin);
    DelNewLine(NomClient);  // Supprime le \n ajouté par fgets

    // Demande de la référence du véhicule et de la quantité
    printf("Reference : ");
    scanf("%d", &reference);
    getchar(); // Consommer le '\n' restant

    printf("Quantite : ");
    scanf("%d", &quantite);
    getchar(); // Consommer le '\n' restant

    // Vérification des valeurs
    if (quantite <= 0) {
        printf("Erreur : Quantité invalide.\n");
        break;
    }

    // Mise à jour du stock et facturation
    MajStockVehiculeHV(reference, quantite);  // Met à jour le stock

    // Si la facturation réussit, un message de confirmation est affiché
    if (FacturationHV("FacturesHV", NomClient, time(NULL), quantite, reference) == 1) {
        printf("Facturation réussie.\n");
    } else {
        printf("Erreur lors de la facturation.\n");
    }
    break;
}

            case '6':
                ListingFacturesHV("FacturesHV");
                break;

            case '7':
                AProposServeurHV("V 1", "Francois & Noah", "Dewez");
                break;

            case '8':
                exit(0);

            default:
                printf("Choix invalide, réessayez.\n");
                break;
        }
    }
}
