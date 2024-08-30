#ifndef STRUCT_H
#define STRUCT_H
#include"common.h"

typedef struct 
{
    //contains pointers to the logic,draw functions
    void (*logic)(void);
    void (*draw)(void);
} Caller;

typedef struct Texture
{
    char name[MAX_NAME_LENGTH];
    SDL_Texture *texture;
    struct Texture *next;
} Texture;

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    //to track input types better explanation in input.c file function keyPressed
    int keys[MAX_KEYBOARD_KEYS];
    int fire;
    Caller caller;
    int highScore;
    Texture textureHead,*textureTail;
    char playerInputText[MAX_LINE_LENGTH];
} App;

typedef struct Entity
{
    float x;
    float y;
    int w;
    int h;

    float dx;
    float dy;
    int health;
    int reload;
    int side;
    SDL_Texture *texture;

    struct Entity *next;  // Still use struct Entity here
} Entity;

typedef struct Explosion
{
    float x,y,dx,dy;
    int r,g,b,a;
    struct Explosion *next;
} Explosion;



typedef struct Debris
{
    float x,y,dx,dy;
    SDL_Rect rect;
    SDL_Texture *texture;
    int life;
    struct Debris *next;
} Debris;

typedef struct 
{
    //Holds information about the fighters,bullets,explosions,debris. 
    Entity fighterHead, *fighterTail;
    Entity bulletHead,*bulletTail;
    Entity pointHead,*pointTail;
    Debris debrisHead,*debrisTail;
    Explosion explosionHead,*explosionTail;
    Entity destroyedHead,*destroyedTail;
    int score;
} Stage;



typedef struct
{
    int x,y,speed;
} Star;

typedef struct 
{
    int recent,score;
    char name[MAX_SCORE_NAME_LENGTH];
} Highscore;

typedef struct
{
    Highscore highscoresArray[NUM_HIGHSCORE];
} Highscores;

#endif