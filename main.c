#include<stdio.h>
#include <stdlib.h>
#include<SDL2/SDL_image.h>
#include"structs.h" //structs already contains common.h which has SDL2 header
#include"draw.h"
#include"init.h"
#include"input.h"

void cleanup();

App app;
Entity player;
int main(void)
{
    
    memset(&app,0,sizeof(App));
    memset(&player,0,sizeof(Entity));
    
    initSDL();

    player.x=110;
    player.y=110;
    player.texture=loadTexture("textures/player.png");

    atexit(cleanup);

    while(1)
    {
        prepareScene();

        doInput();

        drawTexture(player.texture,player.x,player.y);

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

    IMG_Quit();
    SDL_Quit();

}