//ELKILANI NARMIN
//PETROV MECHAEL

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

int recherche_element(int valeur, int tab[],int indice_ignore)
{

// on recherche si i un element est dans un tableau et on retourne son indice ou -1
  for (int i = 0; i < TAILLE_PLAT; i++)
  {
    if (valeur == tab[i] && i != indice_ignore)//cette fonction est lancée seulement quand on cherche un autre joueur dans le trou donc on doit ignoré le joueur original dans le trou
      return i;
  }
  return -1;
}

void collision(char plateau[], int positions[], int attente[], int nb_joueurs,
               int joueur_courant, int nouvelle_pos, int des[2])// on teste la collision entre les joueurs
{
  /* la fonction prend positions,attente qui n'a pas encore changé et
   ne change pas la position de joueur courant a nouvelle_pos */

  if (plateau[nouvelle_pos] != 'P' && plateau[nouvelle_pos] != 'T')
  { int joueur_deplace;
    // si la case n'est pas trou ou prison
    int i = 0;
    while (i < nb_joueurs)
    {
      if (nouvelle_pos == positions[i])
      {
        joueur_deplace = i;
        positions[joueur_deplace] = positions[joueur_courant-1];
        // on teste sur position(joueur_courant -1) car si j_courant est 1 sa position est pos[0]
        break;
      }

      i++;
    }
  }
}

void appliquer_effet_cases(char plateau[], int positions[], int nb_joueurs, int attente[], int joueur_courant, int des[2], int *nouvelle_pos)
{
  char cas = plateau[*nouvelle_pos];

  switch (cas)
  {
  case 'O':
    *nouvelle_pos += des[0] + des[1];
    printf("Le joueur atterie sur une case oie !\n");
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'H':
    printf("Le joueur atterie sur un hotel !\n");
    attente[joueur_courant] = 2;
    break;

  case 'L':
    printf("Le joueur ce perd dans le labyrinte !\n");
    *nouvelle_pos = 52;
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'X':
    *nouvelle_pos = 0;
    printf("Le joueur tombe dans le trou !\n");
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;

  case 'R':
    *nouvelle_pos = 16;
    printf("Le joueur prend le racourci !\n");
    collision(plateau, positions, attente, nb_joueurs, joueur_courant, *nouvelle_pos, des);
    break;
  
  case 'T':
    attente[joueur_courant] = -1;printf("le joueur tombe dans le trou!\n");
    if (recherche_element(53, positions,joueur_courant)){
      attente[joueur_courant] = 0;} // si un autre joueur est dans le trou, il sort
  
  case 'P':
    printf("le joueur est en prison !!\n");
  if (recherche_element(74, positions,joueur_courant))
      {printf("les joueur en prison ce sont echapé !!\n");
      attente[recherche_element(74, positions,joueur_courant)] = 0;  attente[joueur_courant] = 0;
    }
    else
    {
      attente[joueur_courant] = -1;
    }
    


  default:
    break;
  }
}


int avancerJoueur(char plateau[], int positions[], int attente[], int joueur_courant, int nb_joueurs, int des[2], int premier_tour)
{
  if (!attente[joueur_courant - 1])
  {
    int nouvelle_pos = positions[joueur_courant - 1] + des[0] + des[1];
    int cas_special = 0;
    if (premier_tour){
      // cas special 4,5
      if ((des[0] == 4 && des[1] == 5) || (des[1] == 4 && des[0] == 5))
      {
        nouvelle_pos = 89;
        cas_special = 1;
        collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
        positions[joueur_courant - 1] = nouvelle_pos;
        appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);
        positions[joueur_courant - 1] = nouvelle_pos;
      }
      // cas special 3,6
      if ((des[0] == 3 && des[1] == 6) || (des[1] == 3 && des[0] == 6))
      { 
        nouvelle_pos = 40;
        collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
        cas_special = 1;
        positions[joueur_courant - 1] = nouvelle_pos;
        appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);
        positions[joueur_courant - 1] = nouvelle_pos;
      }
    }
    if(!cas_special) // quand pas de cas spécial pour le 1er tour
    {
      if (nouvelle_pos > 99) // si on depase 100
        nouvelle_pos = 99 - nouvelle_pos % 99;
      collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
      positions[joueur_courant - 1] = nouvelle_pos;
      appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);
      positions[joueur_courant - 1] = nouvelle_pos;
    }
  }else
    {
     //printf("le joueur attend son tour !\n");
    
    }if(positions[joueur_courant]==99){return joueur_courant;}
  return -1;
}


int joueur_qui_minimise(int j1,int j2,int joueur_courant,int nb_joueur){
  //pour savoir quel joueur est prioritaire a l'affichage si meme case (0 si j1, 1 si j2)
  if(j1==1000){return 1;}
  if((j1-joueur_courant)%nb_joueur < (j2-joueur_courant)%nb_joueur)
  {return 0;}
  else{return 1;}
}

void afficherPlateau(char plateau[], int positions[], int nb_joueurs,int joueur_courant){//doit ajouter joueur courant pour savoir lequel afficher
  int temp[10][10] = {
    {0,1,2,3,4,5,6,7,8,9},
    {35,36,37,38,39,40,41,42,43,10},
    {34,63,64,65,66,67,68,69,44,11},
    {33,62,83,84,85,86,87,70,45,12},
    {32,61,82,95,96,97,88,71,46,13},
    {31,60,81,94,99,98,89,72,47,14},
    {30,59,80,93,92,91,90,73,48,15},
    {29,58,79,78,77,76,75,74,49,16},
    {28,57,56,55,54,53,52,51,51,17},
    {27,26,25,24,23,22,21,20,19,18}};
  int joueur_aff;
  for(int i=0;i<10;i++){
    for(int j=0;j<10;j++){
      joueur_aff = 1000;
      char caze = plateau[temp[i][j]];
      if(caze==' '){caze='_';}
      //ici pour trouver si un joueur et sur la case et afficher
      //int joueura_affic; for i in nbjoueur: if(position[nbjoueur]==temp[i][j]{   if joueur_qui_min() {joueurach = j2}else j1   })
      for(int y=0;y<nb_joueurs;y++){
        if(positions[y]==temp[i][j]){
          if(joueur_qui_minimise(joueur_aff,y,joueur_courant,nb_joueurs)){
              joueur_aff = y;
          }
        }
      }
      if(joueur_aff!=1000){caze=joueur_aff+'0'+1;}//1000 est la valeur pour dire pas de joueur à afficher
      printf("%c ",caze);
    }printf("      ");
    for(int l=0;l<10;l++){
      if(temp[i][l]<10){printf(" ");}
      printf("%d ",temp[i][l]);
    }
    printf("\n");
  }printf("\n");

  for(int k = 0; k < nb_joueurs; k++){
    printf("Joueur %d: case %d\n",k+1,positions[k]);
  }
  printf("\n\n\n");
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
    int nb_tours=0; //nb_tours = numéro de ligne-1 dans le fichier
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
    printf("Le nombre de joueurs doit etre entre 2 et 4\n");
    scanf("%d", &nb_joueurs);
  }
  //afficherPlateau( plateau, positions,nb_joueurs);
  // intialisation des tab positions[] et attente[]
  int positions[4] = {};    // on met 4 pour ne pas initialiser par une variable avec max possible est 4
  int attente[4] = {};       // attente est initialisé à 0
  
  fprintf(fileW, "%d JO", nb_joueurs);
  printf("Pour chaque tour, indiquer les valeurs des deux des ou taper q pour quitter\n");



  // debut du jeu
  int premier_tour = 1;
  int joueur_courant = 1;
  int gagnant = -1;
  while (gagnant == -1)
  {
    joueur_courant = 1;
    for (joueur_courant = 1; joueur_courant <= nb_joueurs; joueur_courant++)
    { 
      if(!attente[joueur_courant])
      {
        printf("lancé de dés du joueur %d: ", joueur_courant);
        scanf("%d", des);

        if (des[0] == -1) // on quitte quand le joueur tappe -1 pour pas q (input buffer erreur engendrée par q)
        {
          printf("Arrêt, partie sauvegardée dans ma_sauvegarde.jo\n");
          exit(0); // on sort du programme
        }

        scanf("%d", des + 1);
        while (des[0] < 1 || des[0] > 6 || des[1] < 1 || des[1] > 6)
        {
          printf("la valeur du des est entre 1 et 6: ");
          scanf("%d %d", des, des + 1);
        }
        fprintf(fileW, "%d %d\n", des[0], des[1]);
        gagnant = avancerJoueur(plateau, positions, attente, joueur_courant, nb_joueurs, des, premier_tour);if(gagnant!=-1)break;
        afficherPlateau( plateau, positions, nb_joueurs,joueur_courant);
    }
      else
      {
        printf("\njoueur %d passe sont tour",joueur_courant);attente[joueur_courant]--;
      }
    }

    premier_tour = 0;
  }
  printf("\nLe joueur %d gagne la partie !!! \n",gagnant);
  fclose(fileW);
  return 0;
}