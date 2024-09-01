#include"background.h"

void initBackground()
{
    backgroundTexture=loadTexture("textures/background.png");
}
//Sets the background to the size of the screen
void drawBackround()
{
    SDL_Rect rect;
    int x;
    for(x=0;x<SCREEN_WIDTH;x+=SCREEN_WIDTH)
    {
        rect.x=x;
        rect.y=0;
        rect.h=SCREEN_HEIGHT;
        rect.w=SCREEN_WIDTH;

        SDL_RenderCopy(app.renderer,backgroundTexture,NULL,&rect);
    }
}

//set the color of the stars and draw them as lines
void drawStarfield(void)
{
    int i,c;
    for(i=0;i<MAX_STARS;i++)
    {
        c=32*stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer,c,c,c,255);

        SDL_RenderDrawLine(app.renderer,stars[i].x,stars[i].y,stars[i].x+3,stars[i].y);
    }
}

//moves every star in the static array, warping them when they reach the end of the screen.
void  moveStarfield()
{
    int i;
    for(i=0;i<MAX_STARS;i++)
    {
        stars[i].x-=stars[i].speed;

        if(stars[i].x < 0)
        {
            stars[i].x=SCREEN_WIDTH+stars[i].x;
        }
    }
}

//makes sure the backround is always in place. 
void  handleBackround()
{
    if(--backroundX < -SCREEN_WIDTH)
    {
        backroundX=0;
    }
}

//Sets the star array with rando star positions
void initStarfield()
{
    int i;
    for(i=0;i<MAX_STARS;i++)
    {
        stars[i].x=rand()%SCREEN_WIDTH;
        stars[i].y=rand()%SCREEN_HEIGHT;
        stars[i].speed=1+(rand()%8);
    }
}