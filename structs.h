#ifndef STRUCT_H
#define STRUCT_H
#include"common.h"

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    //to track input types better explanation in input.c file function keyPressed
    int up,down,left,right;
    int fire;
} App;

typedef struct
{
    int x;
    int y;
    
    // dx,dy is the delta of x,y
    int dx;
    int dy;
    int health;
    SDL_Texture *texture;
} Entity;
#endif // STRUCT_H