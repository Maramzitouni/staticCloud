/*H**********************************************************************
 * FILENAME: miscs.utils.h
 *
 * DESCRIPTION :
 *       Miscellaneous things for our app
 *
 * NOTES :
 *       Copyright Group 7. 2022. All rights reserved.
 *
 * AUTHORS :  ZITOUNI Maram ,RIVIERE Julien, MOUGNI Nawel
 * START DATE : 07/10/2022
 *
 *H***********************************************************************/

/*=====================================================*/
#include <SDL2/SDL.h>
#include <string.h>


typedef struct{
    SDL_Texture* fontTexture;
    SDL_Surface* game;
    SDL_Texture* menuTexture;
}Menu;

typedef struct{
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *menu;
    SDL_Texture *playerTex;
    SDL_Surface *playerSurface;
    SDL_Rect srcR,destR;
    Menu menuProps;
}Game;

typedef struct{
    SDL_Rect src,dest;
    SDL_Texture* wallD;
    SDL_Texture* wall;
    int nbJoueurMax;
    char* nomFichier;
    int largeurMap;
    int hauteurMap;
    int tabMap[20][25];
    char** contenue;
}Map;

typedef struct{
    int xpos;
    int ypos;

    SDL_Texture* objTexture;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    char vie;
    char armure;
    char bombeStocke;
    char degatExplosion;
    char tailleExplosion;
}GameObject;

typedef struct {
    Game *game;
    GameObject *gameObject;
    Map*map1;
    GameObject *object;
}thread_struct;

