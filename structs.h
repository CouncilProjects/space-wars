#ifndef STRUCT_H
#define STRUCT_H
#include"common.h"

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct
{
    int x;
    int y;
    SDL_Texture *texture;
} Entity;
#endif // STRUCT_H