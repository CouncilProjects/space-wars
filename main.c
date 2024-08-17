#include<SDL2/SDL.h>
#include<stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720



typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

void initSDL();
void doInput();
void prepareScene();
void presentScene(); 
void cleanup();

App app;

int main(void)
{
    
    memset(&app,0,sizeof(App));

    initSDL();
    atexit(cleanup);

    while(1)
    {
        prepareScene();

        doInput();

        presentScene();

        SDL_Delay(18);
    }

    return 0;
}

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

void doInput()
{
    SDL_Event event;

    //take events as long as there are
    //for now only react to closing
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        
        default:
            break;
        }
    }
}

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

void cleanup()
{
    if(app.renderer)
    {
        SDL_DestroyRenderer(app.renderer);
    }

    if(app.window)
    {
        SDL_DestroyWindow(app.window);
    }

    SDL_Quit();

}