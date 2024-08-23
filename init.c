#include"init.h"
#include "SDL_image.h"
#include"SDL_mixer.h"
extern App app;// i tell it there is indeed an app somewhere. 

//Sets up the SDL enviroment
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

    //-1 indicates to use the first available renderer driver it finds that supports the requirments
    app.renderer=SDL_CreateRenderer(app.window,-1,rendererFlags);
    if(!app.renderer)
    {
        printf("Failed to create renderer\n");
        exit(1);
    }
    
    // Initilize image, make it able to load png and jpg
    if(!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
    {
        printf("Failed to initialize image loader, %s\n",SDL_GetError());
    }
    
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Error","Could not open audio",NULL);
        exit(1);
    }

    //how many different sounds can play
    Mix_AllocateChannels(MAX_SND_CHANNELS);
}