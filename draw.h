#ifndef DRAW_H
#define DRAW_H
#include<SDL2/SDL_image.h>
#include"structs.h"
void prepareScene();
void presentScene(); 

//return pointer to the loader texture
SDL_Texture *loadTexture(char* fileName);
//Draw a texture at the coordinates x,y
void drawTexture(SDL_Texture *texture,int x,int y);
#endif