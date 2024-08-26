#ifndef BACKG_H
#define BACKG_H

#include"common.h"
#include"defs.h"
#include"structs.h"
#include"draw.h"
extern App app;

static int backroundX;
static Star stars[MAX_STARS];
static SDL_Texture *backgroundTexture;

void initBackground();

//Sets the background to the size of the screen
void drawBackround();


//set the color of the stars and draw them as lines
void drawStarfield(void);

//moves every star in the static array, warping them when they reach the end of the screen.
void  moveStarfield();


//makes sure the backround is always in place. 
void  handleBackround();

//Sets the star array with rando star positions
void initStarfield();

#endif