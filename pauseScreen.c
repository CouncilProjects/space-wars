#include"pauseScreen.h"

void pause()
{
    if(app.keys[SDL_SCANCODE_P]!=1)
    {
        return;
    }
    //clear the input so as to not immidiately change the screen. 
    memset(app.keys,0,MAX_KEYBOARD_KEYS);

    SDL_SetRenderDrawColor(app.renderer,0,0,0,255);//make a semitransparent layer

    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.h=SCREEN_HEIGHT;
    rect.w=SCREEN_WIDTH;

    while (app.keys[SDL_SCANCODE_SPACE]!=1)
    {
        doInput();
        SDL_RenderFillRect(app.renderer,&rect);
        drawText(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,text_center,200,120,100,"PAUSED GAME PRESS SPACE TO CONTINIUE");
        SDL_RenderPresent(app.renderer);
    }
    
    //clear all input that pahhend while in pause
    memset(app.keys,0,MAX_KEYBOARD_KEYS);
}

