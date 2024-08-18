#include"init.h"

extern App app;// i tell it there is indeed an app somewhere. 

void initSDL()
{

    
    //use hardware for accelaration
    int rendererFlags=SDL_RENDERER_ACCELERATED;

    //set the sdl video subsystem
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("error in init error %s\n",SDL_GetError());
        exit(1);
    }

    app.window=SDL_CreateWindow("Shooter",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,0);

    if(!app.window)
    {
        printf("Failed to open window %s\n",SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");

    app.renderer=SDL_CreateRenderer(app.window,-1,rendererFlags);
    if(!app.renderer)
    {
        printf("Failed to create renderer\n");
        exit(1);
    }

}