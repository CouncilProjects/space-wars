#ifndef DRAW_H
#define DRAW_H
#include"SDL_image.h"
#include"structs.h" //already brings def.h

//Draws the color of the scene.
void prepareScene();

//Draw all new renderings that happend
void presentScene(); 

//Load the image from the specifide path, and make it to a texture Parameter[the path of the image] Return[Pointer to the image turned into SDL_texture]
SDL_Texture *loadTexture(char* fileName);

//checks if texture is already loaded
static SDL_Texture* getTexture(const char* filename);

//add the texture to the app texture list. 
static void addToCache(char* filename,SDL_Texture* texture);

//Draw a texture at the coordinates x,y
void drawTexture(SDL_Texture *texture,int x,int y);

//Draw the src part of the given texture at coordinates x,y.
void drawPartialTexture(SDL_Texture *texture,SDL_Rect *src,int x,int y);

//Draws an entire texture and displays it in a modified size increased by addexW X addedH at coordinates x,y.
void drawModifiedTexture(SDL_Texture *texture,int x,int y,int addedW,int addedH);

//Draws a part of a texture and displays it in a modified size increased by addexW X addedH at coordinates x,y.
void drawPartialModifiedTexture(SDL_Texture *texture,SDL_Rect *src,int x,int y,int addedW,int addedH);
#endif