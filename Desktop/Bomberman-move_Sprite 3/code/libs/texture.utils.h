#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>


SDL_Texture* LoadTexture(const char* texture,Game*game){

    SDL_Surface * tempSurface= SDL_LoadBMP(texture);
    SDL_Texture* tex= SDL_CreateTextureFromSurface(game->renderer,tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}




void Draw(SDL_Texture* tex,SDL_Rect src,SDL_Rect dest,Game*game){
    SDL_RenderCopy(game->renderer,tex,&src,&dest);
}

/*void InitMenu(SDL_Texture* tex,Game*game){
    SDL_RenderCopy(game->renderer,tex,NULL,NULL);
}*/


/*SDL_Texture* LoadFont(const char* fontName,const char* text,Game*game) {
    TTF_Font *font = TTF_OpenFont(fontName, 25);
    SDL_Color color = {255, 255, 255,255};
    SDL_Surface *surface = TTF_RenderText_Solid(font,text, color);
    SDL_Texture *tex= SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    return tex;
}*/