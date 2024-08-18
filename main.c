#include<stdio.h>
#include <stdlib.h>
#include"structs.h" //structs already contains common.h which has SDL2 header
#include"draw.h"
#include"init.h"
#include"input.h"

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