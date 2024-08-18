#include "draw.h"

extern App app;// i tell it there is indeed an app somewhere. 

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture *loadTexture(char* fileName)
{
    SDL_Texture *texture;
    texture=IMG_LoadTexture(app.renderer,fileName);
    return texture;
}

void drawTexture(SDL_Texture *texture,int x,int y)
{
    SDL_Rect dest;
    dest.x=x;
    dest.y=y;
    //find the attributes of the texture (so that we know the w and h)
    SDL_QueryTexture(texture,NULL,NULL,&dest.w,&dest.h);

    //render a copy of the tecture. The null argument tell it to render it whole 
    SDL_RenderCopy(app.renderer,texture,NULL,&dest);
}