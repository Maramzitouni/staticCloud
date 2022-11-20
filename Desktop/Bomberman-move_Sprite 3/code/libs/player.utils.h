#include <stdlib.h>
#include <string.h>


#include <stdlib.h>
#include <string.h>
//#include "../libs/miscs.utils.h"

typedef struct player player;
struct player
{
    // char bombeStocke;       //démarre a 3, le joueur ne peux plus placer bombe si il n'y en a plus
    // char degatExplosion;    //démarre a 1, peut varier de 1 (ou 0?) a 3;
    // char tailleExplosion;   //démarre a 3 allant de 1 a 5
    char vie;               // max 5, a 0 tu sort.
    char armure;             // = bool, si true alors ignore dégat (explosion a 0 dégat enlève ?)

    //placer asset joueur ici
    //mettre un pointeur GameObject en argument ou copier gameObjetct?

    GameObject* objectPlayer;
    player* next;
    player* prev;
    //SDL_Rect src,dest;  //ça correspond a quoi?
};

player* initJoueur(){ //on pourrait init l'objetJoueur ici aussi mais j'aurais besoin d'aide
    //attention l'objet créé ici n'est pas free, a free après
    player* joueur = malloc(sizeof(player));
    joueur->objectPlayer= malloc(sizeof(GameObject));
    joueur->vie = 3;
    joueur->armure = 0;
    joueur->next = joueur;
    joueur->prev = joueur;
    return joueur;
}




void copyPlayer(player* cible,player* joueurACopier){

    cible->objectPlayer->xpos = joueurACopier->objectPlayer->xpos;
    cible->objectPlayer->ypos = joueurACopier->objectPlayer->ypos;
    cible->next = joueurACopier->next;
    cible->prev = joueurACopier->prev;
    //cible->objectPlayer = joueurACopier->objectPlayer;    //il faut copier les arguement de objectPlayer, pas le pointeur
    cible->armure = joueurACopier->armure;
    cible->vie = joueurACopier->vie;

    //return cible;
}

player* seekPlayer(player* listeJoueur,int x,int y){    //cherche un joueur en particulier grace a ses coordonnés, renvoie NULL si le joueur n'existe pas au coordonnés inscrites

    player* result;
    if(listeJoueur == NULL){
        printf("attention la liste de joueur ne contiens personne, veuillez vérifier la liste de joueur.");
        return NULL;
    }
    int saveX = listeJoueur->objectPlayer->xpos;
    int saveY = listeJoueur->objectPlayer->ypos; //2 joueur ne peuvent avoir les même coordonnés
    do{
        listeJoueur = listeJoueur->next;
    }while((listeJoueur->objectPlayer->xpos != x && listeJoueur->objectPlayer->ypos != y) && (listeJoueur->objectPlayer->xpos != saveX && listeJoueur->objectPlayer->ypos != saveY));
    result = listeJoueur;

    while(listeJoueur->objectPlayer->xpos != saveX && listeJoueur->objectPlayer->ypos != saveY){  //pb possible : remettre listeJoueur a son état d'origine quand on a fini de chercher
        //TOVERIFY
        listeJoueur = listeJoueur->next;
    }

    if(listeJoueur->objectPlayer->xpos == saveX && listeJoueur->objectPlayer->ypos == saveY) return NULL;
    return result;

}

void retirerJoueur(player* JoueurARetirer){
    if(JoueurARetirer == NULL) return;
    free(JoueurARetirer->objectPlayer);
    player* save = JoueurARetirer;
    JoueurARetirer->prev->next = JoueurARetirer->next;
    JoueurARetirer->next->prev = JoueurARetirer->prev;
    JoueurARetirer = JoueurARetirer->next;
    free(save);
}

void retirerJoueurEmplacement(player* listeJoueur, int x, int y){
    retirerJoueur(seekPlayer(listeJoueur,x,y));
}

void retirerTousLesJoueurs(player* listeJoueur){    //permet de free l'enssemble des joueur de la liste ainsi que leur attribut
    // int saveX = listeJoueur->prev->x;
    // int saveY = listeJoueur->prev->y;
    while(listeJoueur != listeJoueur->next){
        printf("l'adresse de ma listejoueur est %p\n",listeJoueur);

        retirerJoueur(listeJoueur);
        listeJoueur = listeJoueur->next;
    }
    printf("l'adresse de ma dernière listejoueur est %p\n",listeJoueur);

    //printf("ma variable x = %d",listeJoueur->x);

    free(listeJoueur->objectPlayer);
    free(listeJoueur);
    //printf("ma variable x = %d",listeJoueur->x);
}

void afficherEmplacementTousLesJoueur(player* listeJoueur){ //FONCTION de DEBUG
    int saveX = listeJoueur->objectPlayer->xpos;
    int saveY = listeJoueur->objectPlayer->ypos;
    do{
        printf("l\'emplacement de ce joueur est a x = %d et y = %d\n",listeJoueur->objectPlayer->xpos,listeJoueur->objectPlayer->ypos);
        listeJoueur = listeJoueur->next;
    }while(listeJoueur->objectPlayer->xpos != saveX && listeJoueur->objectPlayer->ypos != saveY);
}

void ajouterJoueur(player* listeJoueur){
    player* NouveauJoueur = initJoueur();
    if(listeJoueur->next != listeJoueur){ //dans ce cas là on a un seul listeJoueur dans la liste
        player* save;   //on ne malloc que pour next ()

        save = listeJoueur->next;

        listeJoueur->next = NouveauJoueur;  //ça m'embete a partir de la, j'ai un doute sur le résutlat que je prévois
        //GHOST_PLAYER <-> NOUVEAU_JOUEUR ! (fin de la boucle) PLAYER1 <-> SAVE || PLAYER2 <-> ...
        NouveauJoueur->next = save;
        //                   GHOST_PLAYER <-  PLAYER1 || SAVE)
        //GHOST_PLAYER <-> NOUVEAU_JOUEUR ->  PLAYER1 || SAVE <-> PLAYER2 <-> ...
        save->prev = NouveauJoueur;
        //GHOST_PLAYER <-> NOUVEAU_JOUEUR <-> PLAYER1 <-> SAVE || PLAYER2 <-> ...
    }
    else{
        listeJoueur->next = NouveauJoueur;
        NouveauJoueur->next = listeJoueur;
    }
    NouveauJoueur->prev = listeJoueur;
}