#include <stdio.h>
#include <stdlib.h>
#define TAILLE_PLAT 100


void creer_plateau(char plateau[TAILLE_PLAT]){ //pour creer le plateau au debut de la partie
    for(int i = 0;i < TAILLE_PLAT; i++){
      plateau[i] = ' ';
      if(!(i%9)){plateau[i] = 'O';}
    }
    plateau[0] = ' ';
    plateau[53] = 'T';
    plateau[8] = 'R';
    plateau[74] = 'P';
    plateau[31] = 'H';
    plateau[65] = 'L';
    plateau[94] = 'X';    
    }

int recherche_element(int valeur, int tab[]) {

  // on recherche is un element est dans un tableau et on retourne l'indice ou 0
    for (int i = 0; i < TAILLE_PLAT; i++) {
        if (valeur == tab[i]) 
            return i; 
        
    }
    return 0; 
} 

void appliquer_effet_cases(char plateau[], int positions[], int nb_joueurs, int attente[], int joueur_courant, int des[2], int *nouvelle_pos) {
    char cas = plateau[*nouvelle_pos];

    switch (cas) {
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
  if (cas  == 'P'){
    if ( recherche_element(74, positions)){
      attente[recherche_element(74, positions) ] = 0;
     
    }else
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



void collision(char plateau[], int positions[], int attente[],int nb_joueurs,
int joueur_courant,int nouvelle_pos, int des[2]){
//la fonction prend positions,attente qui n'a pas encore changé et 
//ne change pas la position de joueur courant a nouvelle_pos

  if( plateau[nouvelle_pos]!='P' && plateau[nouvelle_pos]!='T'){
  //si la case n'est pas trou ou prison
    int i = 0;
    while(i < nb_joueurs){
      if(nouvelle_pos == positions[i]){ 
        i--;
      break;}

    i++;
    }
    if(i < nb_joueurs){
    //vrai si il y a eu une colision et i est le joueur sur lequel on a atterit
      int joueur_deplace = i;
      positions[joueur_deplace - 1] = positions[joueur_courant -1];// on teste sur position(joueur_courant -1) car si j_courant est 1 sa position est pos[0]
      appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);
      // le joueur deplacé a les effets de la case apliqué
      }
    }
}



int avancerJoueur(char plateau[], int positions[], int attente[],int joueur_courant, int nb_joueurs, int des[2], int premier_tour){
 

    // si le joueur est dans la case hotel et il lui reste 1 tour pour sortir
    if(attente[joueur_courant - 1] == 2)
      attente[joueur_courant - 1] = 1;
  
    //si le joueur est dans la case hotel et il lui reste 0 tour pour sortir
    if(attente[joueur_courant - 1] == 1)
    attente[joueur_courant - 1] = 0; // attente[0] correspond a joueur 1 

    if(!attente[joueur_courant - 1]) {
      int nouvelle_pos;

      if(premier_tour){
        if((des[0] == 4 && des[1] == 5) || (des[1] == 4 && des[0] == 5)) // cas special 4,5
        nouvelle_pos = 89;
        else if((des[0] == 3 && des[1] == 6) || (des[1] == 3 && des[0] == 6)) // cas special 3,6
        nouvelle_pos = 40;
        collision(plateau,positions,attente,nb_joueurs,joueur_courant,nouvelle_pos, des);
        positions[joueur_courant - 1] = nouvelle_pos;
       }
      else
      {
        nouvelle_pos = positions[joueur_courant - 1] + des[0] + des[1];
        if(nouvelle_pos > 99)//si on depase 100
            nouvelle_pos = 99 - nouvelle_pos % 99;
        collision(plateau, positions, attente, nb_joueurs, joueur_courant, nouvelle_pos, des);
        appliquer_effet_cases(plateau, positions, nb_joueurs, attente, joueur_courant, des, &nouvelle_pos);

        positions[joueur_courant - 1] = nouvelle_pos;
      positions[joueur_courant - 1] = nouvelle_pos;
      if(nouvelle_pos == 99){
        return joueur_courant;
      }
      else{
        return -1;
      }

    }
}


}

int main(){
    char plateau[TAILLE_PLAT];
    creer_plateau(plateau);



    return 0;
}