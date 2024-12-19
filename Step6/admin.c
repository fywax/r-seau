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
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static struct termios old, new;

// clrscr() function definition
void ClrScr(void)
{
    system("clear");
}

void Gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &old); // grab old terminal i/o settings
    new = old;          // make new settings same as old settings
    new.c_lflag &= ~ICANON; // disable buffered i/o
    new.c_lflag &= echo ? ECHO : ~ECHO; // set echo mode
    tcsetattr(0, TCSANOW, &new); // apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char Getch(void)
{
    return getch_(0);
}

/* Read 1 character with echo */
char GetchE(void)
{
    return getch_(1);
}

// permet de supprimer le cr qui placé dans la chaine lors d'un fgets
void DelNewLine(char *Chaine)
{
    Chaine[strlen(Chaine) - 1] = 0;
}

void MonPrintf(char *tempo, int espace, int taille)
{
    int Count;
    printf("%s", tempo);
    Count = espace - taille;
    while (Count > 0)
    {
        printf(" ");
        Count--;
    }
}

void AfficheEnteteVehiculeHV()
{
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

void AfficheVehiculeHV(struct VehiculeHV *UnRecord)
{
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

void SaiSieVehiculeHV(int Reference, struct VehiculeHV *UnRecord)
{
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
    return;
}

// Correction dans la fonction facturation
struct FactureHV *facturation(struct VehiculeHV *vehicule)
{
    struct FactureHV *record = malloc(sizeof(struct FactureHV));
    if (record == NULL)
    {
        perror("Erreur allocation mémoire");
        return NULL;
    }

    printf("Nom du client : ");
    scanf("%s", record->Acheteur);

    printf("Référence : ");
    scanf("%d", &record->Reference);

    printf("Quantité (de véhicules à acheter) : ");
    scanf("%d", &record->Quantite);

    // Vérification que la quantité demandée est disponible dans vehicule
    if (record->Quantite > vehicule->Quantite)
    {
        printf("Erreur : pas assez de véhicules disponibles.\n");
        free(record);
        return NULL;
    }

    // Mise à jour de la quantité de véhicules
    vehicule->Quantite -= record->Quantite;

    return record;
}

// Fonction main corrigée
int main()
{
    char Choix;
    char Tampon[80];
    int res;
    struct VehiculeHV UnRecord;
    int Numero, reference;
    while (1)
    {
        printf("-------2022-----------\n");
        printf("1) Ajout              \n");
        printf("2) Vehicule           \n");
        printf("4) Recherche          \n");
        printf("5) Achat              \n");
        printf("6) Factures           \n");
        printf("7) A propos           \n");
        printf("8) exit               \n");
        printf("----------------------\n");
        printf(">>");
        Choix = GetchE();
        printf("\n");
        switch (Choix)
        {
        case '1':
        {
            struct VehiculeHV UnRecord;
            FILE *sortie;
            char Redo;

            Redo = 'y';
            while (Redo == 'Y' || Redo == 'y')
            {
                int Nombre;
                Nombre = NombreVehiculesHV("VehiculesHV");
                SaiSieVehiculeHV(Nombre + 1, &UnRecord);
                CreationAjoutVehiculeHV("VehiculesHV", &UnRecord);
                printf("Encoder un autre (Y/N) ?)");
                printf(">>");
                Redo = GetchE();
                printf("\n");
            }

            break;
        }
        case '2':
            ListingVehiculesHV("VehiculesHV");
            break;
        case '4':
            printf("Reference : ");
            scanf("%d", &reference);
            rechercheHV("VehiculesHV", reference, &UnRecord);
            AfficheEnteteVehiculeHV();
            AfficheVehiculeHV(&UnRecord);
            break;
        case '5':
        {
            struct FactureHV *facture = facturation(&UnRecord); // Passer UnRecord en argument
            if (facture != NULL)
            {
                AfficheFacture(facture);
                free(facture); // Libérez la mémoire après utilisation
            }
            else
            {
                printf("Erreur lors de la création de la facture.\n");
            }
            break;
        }
        case '6':
            ListingFacturesHV("FactureHV");
            break;
        case '7':
            AProposServeurHV("V 1", "Francois&Noah", "Dewez");
            break;

        case '8':
            exit(0);
        }
    }
}
