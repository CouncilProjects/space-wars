#ifndef INIT_H
#define INIT_H
#include"highscore.h" //already has structs.h
#include "SDL_image.h"
#include"SDL_mixer.h"
#include"background.h"
extern App app;// i tell it there is indeed an app somewhere. 
////Sets up the SDL enviroment
void initSDL();
void initGame();
#endif