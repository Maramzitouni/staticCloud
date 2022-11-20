#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//--------------------------------------------------------
//  La def de la structure n'est sans doute pas a la bonne place, toute cette partie est necessaire pour le reste du code donc a voir si il faut la bouger
//  au pire faudras faire un include "nouveaufichier.ext"

typedef struct carte carte;
struct carte
{   
    int nbJoueurMax;
    char* nomFichier;
    int largeurMap;
    int hauteurMap;
    char** contenue;
};

carte CartePourJouer;

//---------------------------------------------------------

void InitCarte(){
    CartePourJouer.contenue = malloc(CartePourJouer.largeurMap * sizeof(char));
    for(int i = 0;i < CartePourJouer.largeurMap;i++){
        CartePourJouer.contenue[i] = malloc(CartePourJouer.hauteurMap * sizeof(char));
    }
}

void KillCarte(){
    for(int i = 0;i < CartePourJouer.largeurMap;i++){
        free(CartePourJouer.contenue[i]);
    }
    free(CartePourJouer.contenue);
    free(CartePourJouer.nomFichier);
}

int TransformerStrEnInt(char* str){ //ne marche pas sur str négatif, a modifier si on veut prendre en compte ce cas de figure
    int result = 0;
    int i = 0;
    while ((*(str + i) >= '0' && *(str + i) <= '9') && i < 4){
        result = (result * 10) + *(str + i) - 0x30; 
    }
    return result;
}

void ChargerCarte(char* nomFichier){  //nomFichier est le nom du fichier contenant la map
    FILE* fichierDeLaMap = fopen(nomFichier,"r");   //ouverture du fichier en lecture seule
    if(fichierDeLaMap == NULL){ //ERREUR
        printf("erreur, impossible d\'ouvrir le fichier %s\n",nomFichier);
        return;
    }
    else{
        //ici on est bon pour commencer lecture
        CartePourJouer.nomFichier = malloc(strlen(nomFichier) * sizeof(char));
        strcpy(CartePourJouer.nomFichier,nomFichier);
        fprintf(fichierDeLaMap,"%c\n",CartePourJouer.nbJoueurMax);
        CartePourJouer.nbJoueurMax = CartePourJouer.nbJoueurMax - 0x30;

        fprintf(fichierDeLaMap,"%d %d\n",CartePourJouer.largeurMap,CartePourJouer.hauteurMap);  //A adapter, je recoit des str alors que je veux int

        //puis on dois lire les infos dans le fichier
        for(int i = 0; i < CartePourJouer.largeurMap; i++){
            fprintf(fichierDeLaMap,"%s\n",CartePourJouer.contenue[i]);
        }   //ici on a charger dans le contenue de la map, la map a jouer tout ce qui se trouvait dans le fichier normalement
    }

}

void afficherObjet(){       //fonction de debuging, a détruire pour rendu
    printf("nbJoueurMax = %d\n", CartePourJouer.nbJoueurMax);
    printf("nom du fichier de la map : %s\n",CartePourJouer.nomFichier);
    printf("dimention de la carte : x = %d, y = %d\n", CartePourJouer.largeurMap, CartePourJouer.largeurMap);
    for(int i = 0; i < CartePourJouer.hauteurMap; i++){
        printf("%s", CartePourJouer.contenue[i]);
    }
}



void InitTotal(){
    InitCarte();
    ChargerCarte("initialisation/mapExemple.txt"); //Mettre la var mapExemple en argument ?
    //TODO
}

void Nettoyage(){   //une fois que l'on veut fermer le jeu il faudras appeler cette methode pour libérer la mémoire utilisé par la carte
    KillCarte();
}


typedef struct player player;
struct player
{
    int x,y;
    // char bombeStocke;       //démarre a 3, le joueur ne peux plus placer bombe si il n'y en a plus
    // char degatExplosion;    //démarre a 1, peut varier de 1 (ou 0?) a 3;
    // char tailleExplosion;   //démarre a 3 allant de 1 a 5
    char vie;               // max 5, a 0 tu sort.
    char armure;             // = bool, si true alors ignore dégat (explosion a 0 dégat enlève ?)

    //placer asset joueur ici
    //mettre un pointeur GameObject en argument ou copier gameObjetct?

    GameObject* ObjetJoueur;
    player* next;
    player* prev;

    //SDL_Rect src,dest;  //ça correspond a quoi?
};

//exemple :
//GHOST_PLAYER <-> PLAYER1 <-> PLAYER2 <-> ... <-> GHOST_PLAYER
//GHOST_PLAYER <-> PLAYER1 (<-> GHOST_PLAYER <-> PLAYER1 <-> ...)
//GHOST_PLAYER <-> NULL

player* initJoueur(char x, char y){ //on pourrait init l'objetJoueur ici aussi mais j'aurais besoin d'aide
    //attention l'objet créé ici n'est pas free, a free après
    player* joueur = malloc(sizeof(player));
    joueur->x = x;
    joueur->y = y;
    joueur->vie = 3;
    joueur->armure = 0;
    joueur->next = joueur;    //espace alloué lors de AjouterJoueur
    joueur->prev = joueur;    //espace déjà alloué par AjouterJoueur (sinon repointe vers lui même)
    return joueur;
}


void CopyPlayer(player* cible,player* joueurACopier){

    cible->x = joueurACopier->x;
    cible->y = joueurACopier->y;
    cible->next = joueurACopier->next;
    cible->prev = joueurACopier->prev;
    cible->ObjetJoueur = joueurACopier->ObjetJoueur;
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
    int saveX = listeJoueur->x;
    int saveY = listeJoueur->y; //2 joueur ne peuvent avoir les même coordonnés
    do{
        listeJoueur = listeJoueur->next;
    }while((listeJoueur->x != x &&listeJoueur->y != y) || (listeJoueur->x != saveX && listeJoueur->y != saveY));
    result = listeJoueur;

    while(listeJoueur->x != saveX && listeJoueur->y != saveY){  //pb possible : remettre listeJoueur a son état d'origine quand on a fini de chercher
        //TOVERIFY
        listeJoueur = listeJoueur->next;
    }

    if(listeJoueur->x == saveX && listeJoueur->y == saveY) return NULL;
    return listeJoueur;

}

void RetirerJoueur(player* JoueurARetirer){
    JoueurARetirer->prev->next = JoueurARetirer->next, JoueurARetirer->next->prev = JoueurARetirer->prev;    //une ',' pour que les opérations soit faites simultanément, ou en tout cas que l'une n'influe pas sur l'autre
    free(JoueurARetirer);
}

void RetirerJoueurX(player* listeJoueur, int x, int y){
    RetirerJoueur(seekPlayer(listeJoueur,x,y));
}

void RetirerTousLesJoueurs(player* listeJoueur){
    while(listeJoueur != NULL){
        RetirerJoueur(listeJoueur);
    }
}

void AfficherEmplacementTousLesJoueur(player* listeJoueur){
    int saveX = listeJoueur->x;
    int saveY = listeJoueur->y;
    do{

    }while(listeJoueur->x != saveX && listeJoueur->y != saveY);
}

void AjouterJoueur(player* listeJoueur,char x, char y){
    player* NouveauJoueur = initJoueur(x,y);
    if(listeJoueur->next != listeJoueur){ //dans ce cas là on a un seul listeJoueur dans la liste
        player* save;   //on ne malloc que pour next ()
        //actuellement SAVE = un nbInconnu, SAVE ne pointe vers rien qui nous intéresse.

        //GHOST_PLAYER <-> PLAYER1 <-> PLAYER2 <-> ...
        //SAVE.next = un nbInconnu
        save = listeJoueur->next;
        //GHOST_PLAYER <-> SAVE || PLAYER1 <-> PLAYER2 <-> ...
        //SI listeJoueur = GHOST_PLAYER dans le cas suivant
        //SAVE pointe vers la même var que PLAYER2
        //GHOST_PLAYER <-> SAVE || PLAYER1 <-> PLAYER2 <-> ...
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