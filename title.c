
#include"title.h"

static SDL_Texture *title;

void initTilte()
{
    app.caller.logic=tLogic;
    app.caller.draw=tDraw;

    memset(&app.keys,0,sizeof(int)*MAX_KEYBOARD_KEYS);

    title=loadTexture("textures/title.png");

}

int b=0,r=200;
static void tLogic()
{
    handleBackround();
    moveStarfield();

    if(b>=250)
    {
        b=0;
    }
    if(r<=5)
    {
        r=200;
    }
    b+=1;
    r-=1;

    if(app.keys[SDL_SCANCODE_SPACE]==1)
    {
        resetStage();
    }

}


static void tDraw()
{
    drawBackround();
    drawTexture(title,350,100);
    drawStarfield();
    
    drawText(650,300,text_center,r,SDL_abs(r-b),b,"PRESS SPACE TO START");
}