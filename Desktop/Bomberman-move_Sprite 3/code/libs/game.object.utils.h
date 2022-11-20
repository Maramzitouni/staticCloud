
void InitGameObject(const char*textureSheet, Game*game, GameObject*gameObject,int x,int y){
    gameObject->objTexture= LoadTexture(textureSheet, game);
    gameObject->xpos=x;
    gameObject->ypos=y;
}

void Update(GameObject*gameObject){

    gameObject->srcRect.h=14; //taille de hauteur
    gameObject->srcRect.w=14;   //taille de largeur
    gameObject->srcRect.x=0;
    gameObject->srcRect.y=0;

    gameObject->destRect.x=gameObject->xpos;
    gameObject->destRect.y=gameObject->ypos;

    gameObject->destRect.w=gameObject->srcRect.w*2;
    gameObject->destRect.h=gameObject->srcRect.h*2;

}
void Render(GameObject*gameObject,Game*game){
    SDL_RenderCopy(game->renderer,gameObject->objTexture,&gameObject->srcRect,&gameObject->destRect);
}
