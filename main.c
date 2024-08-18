#include<stdio.h>
#include <stdlib.h>
#include<SDL2/SDL_image.h>
#include"structs.h" //structs already contains common.h which has SDL2 header
#include"draw.h"
#include"init.h"
#include"input.h"

void cleanup();
void move();

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

        //move now can move the player while the key is pressed
        move();

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

void move()
{
    if(app.up==1)
    {
        player.y-=5;
    }

    if(app.down==1)
    {
        player.y+=5;
    }

    if(app.left==1)
    {
        player.x-=5;
    }

    if(app.right==1)
    {
        player.x+=5;
    }
    //this part was for the test of the booster.
    /*else if(app.right==2)
    {
        player.x+=10;
    }*/
}