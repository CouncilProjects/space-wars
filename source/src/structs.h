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
    SDL_Texture *texture; 
    struct Texture *next; 
    char name[MAX_NAME_LENGTH];
} Texture;

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    Texture textureHead;
    Texture *textureTail; 
    int fire;      
    int highScore;       
    int keys[MAX_KEYBOARD_KEYS];       
    Caller caller;      
    char playerInputText[MAX_LINE_LENGTH];    
} App;

typedef struct Entity
{
    float x;
    float y;
    float dx;
    float dy;

    int w;
    int h;
    int health;
    int reload;
    int side;

    SDL_Texture *texture;
    struct Entity *next;
} Entity;

typedef struct Explosion
{
    float x;
    float y;
    float dx;
    float dy;

    int r;
    int g;
    int b;
    int a;

    struct Explosion *next;
} Explosion;



typedef struct Debris
{
    float x;
    float y;
    float dx;
    float dy;

    SDL_Rect rect;
    SDL_Texture *texture;

    int life;
    struct Debris *next;
} Debris;

typedef struct 
{
    // Entity lists
    Entity fighterHead,*fighterTail;
    Entity bulletHead,*bulletTail;
    Entity pointHead,*pointTail;
    Entity destroyedHead,*destroyedTail;

    // Debris and Explosion lists
    Debris debrisHead,*debrisTail;
    Explosion explosionHead,*explosionTail;

    // Score
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