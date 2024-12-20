#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LibSerHV.h"
#include "data.h"

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

// Retourne le nombre total de véhicules dans le fichier
int NombreVehiculesHV(const char *NomFichier) {
    FILE *fichier = fopen(NomFichier, "rb");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return -1;
    }

    fseek(fichier, 0, SEEK_END);
    int Nombre = ftell(fichier) / sizeof(struct VehiculeHV);
    fclose(fichier);

    return Nombre;
}

// Ajoute un nouveau véhicule dans le fichier
void CreationAjoutVehiculeHV(const char *NomFichier, const struct VehiculeHV *UnRecord) {
    FILE *fichier = fopen(NomFichier, "ab");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return;
    }

    fwrite(UnRecord, sizeof(struct VehiculeHV), 1, fichier);
    fclose(fichier);
}

// Recherche un véhicule par référence
int RechercheHV(const char *NomFichier, int Reference, struct VehiculeHV *UnRecord) {
    FILE *fichier = fopen(NomFichier, "r+b");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return 0;
    }

    while (fread(UnRecord, sizeof(struct VehiculeHV), 1, fichier)) {
        if (UnRecord->Reference == Reference) {
            fclose(fichier);
            return 1; // Trouvé
        }
    }

    fclose(fichier);
    return 0; // Non trouvé
}

// Réserve une quantité d'un véhicule si possible
int ReservationHV(const char *NomFichier, int Reference, int Quantite) {
    FILE *fichier = fopen(NomFichier, "r+b");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return 0;
    }

    struct VehiculeHV UnRecord;
    while (fread(&UnRecord, sizeof(struct VehiculeHV), 1, fichier)) {
        if (UnRecord.Reference == Reference) {
            if (UnRecord.Quantite >= Quantite) {
                UnRecord.Quantite -= Quantite;
                fseek(fichier, -sizeof(struct VehiculeHV), SEEK_CUR);
                fwrite(&UnRecord, sizeof(struct VehiculeHV), 1, fichier);
                fclose(fichier);
                return 1; // Réservation réussie
            }
            fclose(fichier);
            return 0; // Stock insuffisant
        }
    }

    fclose(fichier);
    return 0; // Référence non trouvée
}
/*
// Mise à jour du stock du véhicule après un achat
void MajStockVehiculeHV(int Reference, int Quantite) {
    fopen("VehiculeHV", "a+b");
    struct VehiculeHV UnRecord;
    if (RechercheHV("VehiculesHV", Reference, &UnRecord) == 1) {
        if (UnRecord.Quantite >= Quantite) {
            fseek(SEEK_SET)
            fwrite(UnRecord.Quantite = UnRecord.Quantite-Quantite) ;
            printf("Stock mis à jour. Nouvelle quantité: %d\n", UnRecord.Quantite);
        } else {
            printf("Erreur : Stock insuffisant.\n");
        }
    } else {
        printf("Erreur : Véhicule introuvable.\n");
    }
}*/

void MajStockVehiculeHV(int Reference, int Quantite) {
    FILE *fp = fopen("VehiculesHV", "r+b"); // Ouverture en mode lecture/écriture binaire
    if (!fp) {
        perror("(Erreur) Impossible d'ouvrir le fichier VehiculeHV");
        return;
    }

    struct VehiculeHV UnRecord;
    int trouve = 0;

    // Lecture séquentielle pour trouver le véhicule correspondant
    while (fread(&UnRecord, sizeof(struct VehiculeHV), 1, fp)) {
        if (UnRecord.Reference == Reference) {
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("(Erreur) Véhicule introuvable.\n");
        fclose(fp);
        return;
    }

    if (UnRecord.Quantite >= Quantite) {
        UnRecord.Quantite -= Quantite;

        // Revenir à la position de départ de l'enregistrement pour le réécrire
        fseek(fp, -sizeof(struct VehiculeHV), SEEK_CUR);

        // Écrire les nouvelles données
        if (fwrite(&UnRecord, sizeof(struct VehiculeHV), 1, fp) != 1) {
            perror("(Erreur) Échec lors de l'écriture dans le fichier");
        } else {
            printf("Stock mis à jour. Nouvelle quantité: %d\n", UnRecord.Quantite);
        }
    } else {
        printf("(Erreur) Stock insuffisant.\n");
    }

    fclose(fp);
}


// Crée une facture
int FacturationHV(const char *NomFichier, const char *NomClient, time_t Date, int Quantite, int Reference) 
{
    FILE *fichier = fopen(NomFichier, "a+b");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return 0;
    }

    struct FactureHV UneFacture = {
        .NumeroFacturation = NombreVehiculesHV(NomFichier) + 1,
        .Quantite = Quantite,
        .Reference = Reference
    };
    strncpy(UneFacture.Acheteur, NomClient, sizeof(UneFacture.Acheteur) - 1);
    UneFacture.DateFacturation = Date;

    fwrite(&UneFacture, sizeof(struct FactureHV), 1, fichier);
    fclose(fichier);

    return 1; // Facturation réussie
}

// Affiche la liste des véhicules
void ListingVehiculesHV(const char *NomFichier) {
    FILE *fichier = fopen(NomFichier, "rb");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return;
    }

    struct VehiculeHV UnRecord;
    while (fread(&UnRecord, sizeof(struct VehiculeHV), 1, fichier)) {
        printf("Reference: %d, Constructeur: %s, Modele: %s, Quantite: %d, Motorisation: %s\n",
               UnRecord.Reference, UnRecord.Constructeur, UnRecord.Modele,
               UnRecord.Quantite, UnRecord.Motorisation);
    }

    fclose(fichier);
}

// Affiche la liste des factures
void ListingFacturesHV(const char *NomFichier) {
    FILE *fichier = fopen(NomFichier, "r+b");
    if (!fichier) {
        perror("Erreur d'ouverture");
        return;
    }

    struct FactureHV UneFacture;
    while (fread(&UneFacture, sizeof(struct FactureHV), 1, fichier)) {
        printf("Numero: %d, Acheteur: %s, Date: %s, Quantite: %d, Reference: %d\n",
               UneFacture.NumeroFacturation, UneFacture.Acheteur, ctime(&UneFacture.DateFacturation),
               UneFacture.Quantite, UneFacture.Reference);
    }

    fclose(fichier);
}

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}