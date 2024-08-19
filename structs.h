#ifndef STRUCT_H
#define STRUCT_H
#include"common.h"

typedef struct 
{
    //contains pointers to the logic,draw functions
    void (*logic)(void);
    void (*draw)(void);
} Caller;

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    //to track input types better explanation in input.c file function keyPressed
    int keys[MAX_KEYBOARD_KEYS];
    int fire;
    Caller caller;
} App;

typedef struct Entity
{
    float x;
    float y;
    int w;
    int h;

    int dx;
    int dy;
    int health;
    int reload;
    int side;
    SDL_Texture *texture;

    struct Entity *next;  // Still use struct Entity here
} Entity;

typedef struct 
{
    //Holds information about the fighters and bullets. 
    Entity fighterHead, *fighterTail;
    Entity bulletHead,*bulletTail;
} Stage;
#endif // STRUCT_H