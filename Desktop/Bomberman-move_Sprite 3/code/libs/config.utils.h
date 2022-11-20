#include <SDL2/SDL.h>
/*H**********************************************************************
 * FILENAME: config.utils.h
 *
 * DESCRIPTION :
 *       Configurations utils for our app
 *
 * NOTES :
 *       Copyright Group 7. 2022. All rights reserved.
 *
 * AUTHORS :  ZITOUNI Maram ,RIVIERE Julien, MOUGNI Nawel
 * START DATE : 07/10/2022
 *
 *H***********************************************************************/


void InitCarte(Map*CartePourJouer,char* nomFichier){
    CartePourJouer->nomFichier = malloc(strlen(nomFichier) * sizeof(char));
    strcpy(CartePourJouer->nomFichier,nomFichier);   //autant l'init la puisqu'on le peut

    FILE* fichierDeLaMap = fopen(CartePourJouer->nomFichier,"r");   //ouverture du fichier en lecture seule
    if(fichierDeLaMap == NULL){ //ERREUR
        printf("erreur, impossible d\'ouvrir le fichier %s\n",CartePourJouer->nomFichier);
        return;
    }
    else{
        //ici on est bon pour commencer lecture
        fscanf(fichierDeLaMap,"%d\n",&CartePourJouer->nbJoueurMax);
        fscanf(fichierDeLaMap,"%d %d\n",&CartePourJouer->largeurMap,&CartePourJouer->hauteurMap);


        //ici il faux donner la place a mon tableau acceuillant les chars
        CartePourJouer->contenue = malloc((CartePourJouer->largeurMap)* sizeof(char*));
        for(int i = 0;i < CartePourJouer->largeurMap;i++){
            CartePourJouer->contenue[i] = malloc((CartePourJouer->hauteurMap) * sizeof(char));//+ '\n' (+ '\0', pour dernier uniquement)
        }

        //puis on dois lire les infos dans le fichier
        for(int i = 0; i < CartePourJouer->largeurMap; i++){
            fscanf(fichierDeLaMap,"%[^\n]\n",CartePourJouer->contenue[i]);
        }
    }

    fclose(fichierDeLaMap);



}



void KillCarte(Map* CartePourJouer){  //destructeur de la carte
    for(int i = 0;i < CartePourJouer->largeurMap;i++){
        free(CartePourJouer->contenue[i]);
    }
    free(CartePourJouer->contenue);
    free(CartePourJouer->nomFichier);
}

void afficherCarte(Map CartePourJouer){       //DEBUG, a détruire pour rendu
    printf("nbJoueurMax = %d\n", CartePourJouer.nbJoueurMax);
    printf("nom du fichier de la map : %s\n",CartePourJouer.nomFichier);
    printf("dimension de la carte : x = %d, y = %d\n", CartePourJouer.largeurMap, CartePourJouer.hauteurMap);
    for(int i = 0; i < CartePourJouer.hauteurMap; i++){
        printf("%s\n", CartePourJouer.contenue[i]);
    }
}