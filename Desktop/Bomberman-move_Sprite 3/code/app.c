
#include <stdbool.h>
#include <stdlib.h>

#include "libs/miscs.utils.h"
#include "libs/map.utils.h"
#include "libs/game.object.utils.h"
#include "libs/game.utils.h"
#include "libs/config.utils.h"


int main(){

    Game *game= calloc(1, sizeof(Game));
    Map *map1= calloc(1, sizeof(Map));

    GameObject *player1= calloc(1, sizeof(GameObject));
    //GameObject *player2= calloc(1, sizeof(GameObject));
    GameObject *object= calloc(1, sizeof(GameObject));
    GameObject *magic= calloc(1, sizeof(GameObject));


    InitCarte(map1,"code/mapExemple.txt");

    int height=map1->hauteurMap*32;
    int width=map1->largeurMap*32;


    init("SDL Game",width,height,false,map1,game,player1);

    while(running(game)){

        handleEvents(game, player1,map1,object,magic);
        update(player1,object,magic);
        render(game,map1,player1,object,magic);

    }
    clean(game,map1,player1,object,magic);
    KillCarte(map1);

    return 0;
}
