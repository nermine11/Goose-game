#include <stdio.h>
#include <stdlib.h>
#define TAILLE_PLAT 100

void creer_plateau(char plateau[TAILLE_PLAT])
{ // pour creer le plateau au debut de la partie
  for (int i = 0; i < TAILLE_PLAT; i++)
  {
    plateau[i] = ' ';
    if (!(i % 9))
    {
      plateau[i] = 'O';
    }
  }
  plateau[0] = ' ';
  plateau[53] = 'T';
  plateau[8] = 'R';
  plateau[74] = 'P';
  plateau[31] = 'H';
  plateau[65] = 'L';
  plateau[94] = 'X';
  plateau[99] = ' ';
}

int recherche_element(int valeur, int tab[])
{

  // on recherche is un element est dans un tableau et on retourne l'indice ou 0
  for (int i = 0; i < TAILLE_PLAT; i++)
  {
    if (valeur == tab[i])
      return i;
  }
  return 0;
}

// declaration de collision pour l'utiliser dans appliquer_effet_cases
void collision(char plateau[], int positions[], int attente[], int nb_joueurs,
               int joueur_courant, int nouvelle_pos, int des[2]);

void appliquer_effet_cases(char plateau[], int positions[], int nb_joueurs, int attente[], int joueur_courant, int des[2], int *nouvelle_pos)
{
  char cas = plateau[*nouvelle_pos];

  switch (cas)
  {
  case 'O':
    *nouvelle_pos += des[0] + des[1];
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'H':
    attente[joueur_courant - 1] = 2;
    break;

  case 'L':
    *nouvelle_pos = 52;
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'X':
    *nouvelle_pos = 0;
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'R':
    *nouvelle_pos = 16;
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  default:
    break;
  }

  // cas de prison
  if (cas == 'P')
  {
    if (recherche_element(74, positions))
    {
      attente[recherche_element(74, positions)] = 0;
    }
    else
    {
      attente[joueur_courant - 1] = -1;
    }
  }
  // cas de trou
  if (cas == 'T')
  {
    attente[joueur_courant - 1] = -1;
    if (recherche_element(53, positions))
      attente[recherche_element(53, positions)] = 0; // si un autre joueur est dans le trou, il sort
  }
}

void collision(char plateau[], int positions[], int attente[], int nb_joueurs,
               int joueur_courant, int nouvelle_pos, int des[2])
{
  /* la fonction prend positions,attente qui n'a pas encore changé et
   ne change pas la position de joueur courant a nouvelle_pos */

  if (plateau[nouvelle_pos] != 'P' && plateau[nouvelle_pos] != 'T')
  {
    // si la case n'est pas trou ou prison
    int i = 0;
    while (i < nb_joueurs)
    {
      if (nouvelle_pos == positions[i])
      {
        i--;
        break;
      }

      i++;
    }
    if (i < nb_joueurs)
    {
      // vrai si il y a eu une colision et i est le joueur sur lequel on a atterit
      int joueur_deplace = i;
      positions[joueur_deplace - 1] = positions[joueur_courant - 1]; // on teste sur position(joueur_courant -1) car si j_courant est 1 sa position est pos[0]
      appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);
      // le joueur deplacé a les effets de la case apliqué
    }
  }
}

int avancerJoueur(char plateau[], int positions[], int attente[], int joueur_courant, int nb_joueurs, int des[2], int premier_tour)
{

  // si le joueur est dans la case hotel et il lui reste 1 tour pour sortir
  if (attente[joueur_courant - 1] == 2)
    attente[joueur_courant - 1] = 1;

  // si le joueur est dans la case hotel et il lui reste 0 tour pour sortir
  if (attente[joueur_courant - 1] == 1)
    attente[joueur_courant - 1] = 0; // attente[0] correspond a joueur 1

  if (!attente[joueur_courant - 1])
  {
    int nouvelle_pos;

    if (premier_tour)
    {
      if ((des[0] == 4 && des[1] == 5) || (des[1] == 4 && des[0] == 5)) // cas special 4,5
        nouvelle_pos = 89;
      else if ((des[0] == 3 && des[1] == 6) || (des[1] == 3 && des[0] == 6)) // cas special 3,6
        nouvelle_pos = 40;
      collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
      positions[joueur_courant - 1] = nouvelle_pos;
    }
    else
    {
      nouvelle_pos = positions[joueur_courant - 1] + des[0] + des[1];
      if (nouvelle_pos > 99) // si on depase 100
        nouvelle_pos = 99 - nouvelle_pos % 99;
      collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
      appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);

      positions[joueur_courant - 1] = nouvelle_pos;
      positions[joueur_courant - 1] = nouvelle_pos;
      if (nouvelle_pos == 99)
      {
        return joueur_courant;
      }
      else
      {
        return -1;
      }
    }
  }
  return -1;
}

void conversion(int pos, int* x, int* y) {
  if(pos < 10){*x = pos;*y=0;pos=100;}
  if(pos < 19){*x=9;*y=pos-9;pos=100;}
  if(pos < 28){*x=27-pos;*y=9;pos=100;}
  if(pos < 36){*x=0;*y=36-pos;pos=100;}
  if(pos < 44){*x=pos-35;*y=1;pos=100;}
  if(pos < 51){*x=8;*y=pos-42;pos=100;}
  if(pos < 58){*x=58-pos;*y=8;pos=100;}
  if(pos < 64){*x=1;*y=65-pos;pos=100;}
  if(pos < 70){*x=pos-62;*y=2;pos=100;}
  if(pos < 75){*x=7;*y=pos-67;pos=100;}
  if(pos < 80){*x=81-pos;*y=7;pos=100;}
  if(pos < 84){;}

//je continueplus tard

}


void afficherPlateau(int plateau[], int positions[], int nb_joueurs)
{
  ;
}


// Fonction pour charger le jeu
void charger_fichier(char *filename, int *nb_joueurs, int des[2])
{
  FILE *fileR = fopen(filename, "r");

  if (fileR == NULL)
  {
    printf("Erreur: %s\n", filename);
    exit(1);
  }
  // voir si le fichier n'est pas vide pour le charger
  fseek (fileR, 0, SEEK_END);
  int size = ftell(fileR);

  if (size)
  {
    fscanf(fileR, "%d JO", nb_joueurs);
    int nb_tours; //nb_tours = numéro de ligne-1 dans le fichier
    char ch;
    while ((ch = fgetc(fileR)) != EOF) {
        if (ch == '\n') 
            nb_tours++;
    }
    nb_tours--;
    printf("Chargement de parties : %d joueurs, %d tours pour %d lancés de dés simulés", *nb_joueurs, nb_tours, (*nb_joueurs) * nb_tours -1);

    fscanf(fileR, "%d %d", &des[0], &des[1]);
  }


  fclose(fileR);
}

int main()
{

  char plateau[TAILLE_PLAT];
  creer_plateau(plateau);
  int nb_joueurs;
  int des[2];
  // creation du fichier ma_sauvegarde.jo
  FILE *fileW;
  fileW = fopen("ma_sauvegarde.jo", "a");
  if (fileW == NULL)
  {
    printf("Erreur: %s\n", "ma_sauvegarde_jo");
    exit(1);
  }

  /*fseek (fileW, 0, SEEK_END);
  int size = ftell(fileW);

  if (size)
  {
    // charger_fichier() // fonctionalité pas terminé
  } */

  printf("Combien de joueurs ? "); // nb_joueurs
  scanf("%d", &nb_joueurs);
  while (nb_joueurs < 2 || nb_joueurs > 4)
  {
    printf("Le nombre de joueurs est entre 2 et 4 ");
    scanf("%d", &nb_joueurs);
  }
  //afficherPlateau( plateau, positions,nb_joueurs);
  // intialisation des tab positions[] et attente[]
  int positions[nb_joueurs];
  int attente[nb_joueurs];
  for (int i = 0; i < nb_joueurs; i++)
  {
    positions[i] = 0;
    attente[i] = 0;
  }
  
  fprintf(fileW, "%d JO", nb_joueurs);
  printf("Pour chaque tour, indiquer les valeurs des deux des ou taper q pour quitter\n");
  // debut du jeu
  while (1)
  {
    int premier_tour = 1;
    int joueur_courant;
    for (joueur_courant = 1; joueur_courant <= nb_joueurs; joueur_courant++)
    {
      printf("Joueur %d: ", joueur_courant);
      scanf("%d %d", des, des + 1);
      while (des[0] < 1 || des[0] > 6 || des[1] < 1 || des[1] > 6)
      {
        printf("la valeur de des est entre 1 et 6 ");
        scanf("%d %d", des, des + 1);
      }
      if (des[0] == -1)
      {
        printf("Arrêt, partie sauvegardée dans ma_sauvegarde.jo\n");
        exit(0); // on sort du programme
      }
      printf("%d %d\n", des[0], des[1]);
      fprintf(fileW, "%d %d\n", des[0], des[1]);
      avancerJoueur(plateau, positions, attente, joueur_courant, nb_joueurs, des, premier_tour);
      // afficherPlateau( plateau, positions, nb joueurs);
    }

    premier_tour = 0;
  }
  fclose(fileW);
  return 0;
}
