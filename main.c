#include<stdio.h>
#include <stdlib.h>
#include<SDL2/SDL_image.h>
#include"structs.h" //structs already contains common.h which has SDL2 header
#include"draw.h"
#include"init.h"
#include"input.h"
#include"stage.h"

void cleanup();
void capFrames();

App app;
Stage stage;
Entity *player;

int main(void)
{   
    long then;
    float remainder;
    
    memset(&app,0,sizeof(App));
    

    initSDL();
    
    atexit(cleanup);

    initStage();

    then=SDL_GetTicks64();
    remainder=0;

    while(1)
    {
        prepareScene();

        doInput();

        app.caller.logic();
        app.caller.draw();

        presentScene();

        capFrames(&then,&remainder);
    }

    return 0;
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

    IMG_Quit();
    SDL_Quit();

}

void capFrames(long *then,float *remainder)
{
    long wait,frameTime;

    wait=16-*remainder;

    *remainder-=(int)*remainder;

    frameTime=SDL_GetTicks()-*then;

    wait-=frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}