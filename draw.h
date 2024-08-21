#ifndef DRAW_H
#define DRAW_H
#include"SDL_image.h"
#include"structs.h"

//Draws the color of the scene.
void prepareScene();

//Draw all new renderings that happend
void presentScene(); 

//Load the image from the specifide path, and make it to a texture Parameter[the path of the image] Return[Pointer to the image turned into SDL_texture]
SDL_Texture *loadTexture(char* fileName);

//Draw a texture at the coordinates x,y
void drawTexture(SDL_Texture *texture,int x,int y);
#endif