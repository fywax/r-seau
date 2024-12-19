#include "LibSerHV.h"
#include "data.h"

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void AProposServeurHV(char *Version, char *Nom1, char *Nom2)
{
  printf("Version : %s \n", Version);
  printf("Nom1 : %s \n", Nom1);
  printf("Nom2 : %s \n", Nom2);
}

int RechercheHV(char* NomFichier, int Reference, struct VehiculeHV *RetourRecord)
{
  fprintf(stderr, "RechercheHV>\n");

  struct VehiculeHV UnRecord;
  int fd, bytesRead, i = 1;

  fd = open(NomFichier, O_RDONLY);

  if (fd == -1)
  {
    fprintf(stderr, "Echec Ouverture\n");
    fprintf(stderr, "RechercheHV<\n");
    return -1;
  }
  else
  {
      fprintf(stderr, "Ouverture reussie\n");
  }

  bytesRead = read(fd, &UnRecord, sizeof(UnRecord));

  while (bytesRead && UnRecord.Reference != Reference)
  {
    fprintf(stderr, "Reference lue %d (%d Bytes) et Position actuelle dans le fichier %ld\n", i, bytesRead, lseek(fd, 0, SEEK_CUR));
    bytesRead = read(fd, &UnRecord, sizeof(UnRecord));
    i++;
  }

  close(fd);
  fprintf(stderr, "RechercheHV<\n");

  if (!bytesRead) return -1;
  else *RetourRecord = UnRecord;

  return 0;
}
