#include "LibSerHV.h"

void AProposServeurHV(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}


int rechercheHV(char*NomFichier, int Reference, struct VehiculeHV *UnRecord)
{
    FILE *fichier = fopen(NomFichier, "rb");
    if(!fichier)
    {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    while(fread(UnRecord, sizeof(struct VehiculeHV), 1, fichier))
    {
        if(UnRecord->Reference == Reference)
        {
            fclose(fichier);
            return 1;
        }
    }

    fclose(fichier);
    return 0;
}
