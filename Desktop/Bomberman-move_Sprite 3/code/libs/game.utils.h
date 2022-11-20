/*H**********************************************************************
 * FILENAME: game.utils.h
 *
 * DESCRIPTION :
 *       Utils components for our app
 *
 * NOTES :
 *       Copyright Group 7. 2022. All rights reserved.
 *
 * AUTHORS :  ZITOUNI Maram ,RIVIERE Julien, MOUGNI Nawel
 * START DATE : 07/10/2022
 *
 *H***********************************************************************/
#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>



void init(const char* title,int width , int height,bool fullscreen,Map *map1,Game *game,GameObject *player){

    int flags=0;
    if(fullscreen){
        flags= SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING)==0) {
        game->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width, height, flags);
        game->renderer= SDL_CreateRenderer(game->window,-1,0);
        if(game->renderer){
            SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
        }
        game->isRunning=true;
    }
    InitGameObject("../code/avatar.bmp", game,player,100,100);
    //InitGameObject("../code/avatar.bmp", game,player2,0,15);

    initMap(map1,game);
}


void menu(Game*game){
    SDL_DestroyRenderer(game->renderer);

    game->renderer= SDL_CreateRenderer(game->window,-1,0);
    if(game->renderer){
        SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
    }
    game->isRunning=true;
    SDL_Surface* image = SDL_LoadBMP("../code/image.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, image);
    SDL_RenderCopy(game->renderer, texture, NULL, NULL);
    SDL_RenderPresent(game->renderer);

}
bool checkTiles1(const SDL_Rect player, Map*map1,int x) {
    if (map1->tabMap[player.y / 32][player.x / 32] == x ||
        map1->tabMap[(player.y) / 32][(player.x + player.w) / 32] == x ||
        map1->tabMap[(player.y + player.h) / 32][(player.x) / 32] == x ||
        map1->tabMap[(player.y + player.h) / 32][(player.x + player.w) / 32] == x)
    {    //si le player se trouve dans une case=1 de la map (mur/texture grise) return true
        return true;
    }
    return false;
}



void blast(GameObject *bombe,Map*map1){
    int xBlastRight = bombe->xpos+32;
    int xBlastLeft = bombe->xpos-32;
    int yBlastUp = bombe->ypos+32;
    int yBlastDown = bombe->ypos-32;

    if (map1->tabMap[yBlastDown/32][bombe->xpos/32] != 0){
        map1->tabMap[yBlastDown/32][bombe->xpos/32] = 0;
    }else if (map1->tabMap[yBlastUp/32][bombe->xpos/32] != 0){
        map1->tabMap[yBlastUp/32][bombe->xpos/32] = 0;
    }else if (map1->tabMap[bombe->ypos/32][xBlastRight/32] != 0){
        map1->tabMap[bombe->ypos/32][xBlastRight/32] = 0;
    }else if (map1->tabMap[bombe->ypos/32][xBlastLeft/32] != 0){
        map1->tabMap[bombe->ypos/32][xBlastLeft/32] = 0;
    }
}

void magicObject(int max, int min,Map*map1, GameObject*player, SDL_Event event,GameObject*magic,Game*game){

    SDL_bool collision = SDL_HasIntersection(&magic->srcRect,&player->srcRect);

    int randomX = rand()%((max+1)-min) + min;
    int randomY = rand()%((max+1)-min) + min;

    printf("%d\n",randomX);
    printf("%d\n",randomY);
    
    // generate map with the coordonates
    map1->tabMap[randomY/32][randomX/32] = 3;
    int randomIndex = rand()%(5);

    if(randomIndex == 4){
        randomIndex = rand()%(10) ;
    }
    InitGameObject("../code/red.bmp", game,magic,randomX,randomY);

    switch (randomIndex){

    case 0:
        //add one bombe to the player
        if(player->bombeStocke < 7 && collision){
            player->bombeStocke++;
        }
            printf("%d bombeStocks\n",player->bombeStocke);
        break;
    case 1:
        //remove one bombe to the player
        if(player->bombeStocke > 0 && collision){
            player->bombeStocke--;
        }

        break;
    case 2:
        //add one to the impact of bombs
        if(player->tailleExplosion < 5 && collision){
            player->tailleExplosion++;
        }
            printf("%d tailleExpo-1\n",player->tailleExplosion);
        
        break;
    case 3:
        //remove one to the impact of bombs
        if(player->tailleExplosion > 0 && collision){
            player->tailleExplosion--;
        }
            printf("%d tailleExpo-1\n",player->tailleExplosion);
        
        break;
    case 4:

        if(collision) {
            //put impact of bombs to the max
            player->tailleExplosion = 5;
        }
            printf("%d tailleExpoMax\n",player->tailleExplosion);
        break;
    case 5:
        //no collision with walls
        break;
    case 6:
        //BombKick
        switch (event.type)
        {
        case SDLK_UP:
            //map1->tabMap
            break;
        case SDLK_DOWN:

            break;
        case SDLK_RIGHT:

            break;
        case SDLK_LEFT:

            break;
        default:
            break;
        }
        break;
    case 7:
        //
        break;
    case 8:
        //survive to one explosion BUT only one
        break;
    case 9:
        //add one life to the player
        player->vie++;
        break;
    
    default:
        break;
    }

}

void* threadFunction(void *args){
    thread_struct *tmp = args;
    InitGameObject("../code/bombe.bmp", tmp->game, tmp->object, tmp->gameObject->xpos, tmp->gameObject->ypos);
    int posx=tmp->gameObject->xpos;
    int posy=tmp->gameObject->ypos;
    sleep(3);
    InitGameObject("../code/pink.bmp", tmp->game, tmp->object, posx, posy);

    blast(tmp->object,tmp->map1);
    return 0;
}




void handleEvents(Game *game, GameObject *gameObject,Map*map1,GameObject *object,GameObject *magic){
    SDL_Event event;
    pthread_t thread;
    thread_struct* args= malloc(sizeof *args);
    args->game = game;
    args->gameObject = gameObject;
    args->map1 = map1;
    args->object = object;

    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                game->isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game->isRunning = false;
                        break;
                    case SDLK_UP:
                        if ((gameObject->ypos < 0) || (checkTiles1(gameObject->destRect, map1,1))) {
                            gameObject->ypos += 16;
                        } else {
                            gameObject->ypos -= 15;
                        }
                        break;
                    case SDLK_DOWN:
                        if ((gameObject->ypos < 0) || (checkTiles1(gameObject->destRect, map1,1))) {
                            gameObject->ypos -= 13.5;
                        } else {
                            gameObject->ypos += 15;
                        }
                        break;
                    case SDLK_RIGHT:
                        gameObject->xpos += 15;
                        if ((gameObject->xpos < 0) || (checkTiles1(gameObject->destRect, map1,1))) {
                            gameObject->xpos -= 13.5;
                        }

                        break;
                    case SDLK_LEFT:
                        if ((gameObject->xpos < 0) || (checkTiles1(gameObject->destRect, map1,1))) {
                            gameObject->xpos += 16;
                        } else {
                            gameObject->xpos -= 15;
                        }
                        break;
                    case SDLK_SPACE:
                        pthread_create(&thread, NULL, threadFunction, args);
                        magicObject(640,0,map1, gameObject, event,magic,game);


                        break;
                    case SDLK_RETURN:
                    menu(game);
                    break;

                    default: break;
                }
            default: break;
        }
    }
}




void update(GameObject* player,GameObject*object,GameObject*magic){
    Update(player);
    Update(object);
    Update(magic);
}

void render(Game*game,Map*map1,GameObject*player,GameObject*object,GameObject*magic){

    SDL_RenderClear(game->renderer);

    DrawMap(map1,game);
    Render(player,game);
    Render(object,game);
    Render(magic,game);

    SDL_RenderPresent(game->renderer);
}
void clean(Game*game,Map*map1,GameObject*player,GameObject*object,GameObject*magic){
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);

    free(game);
    free(map1);
    free(player);
    free(object);
    free(magic);
    SDL_Quit();
}
bool running(Game*game){return game->isRunning;}


