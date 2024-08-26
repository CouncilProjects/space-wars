#ifndef INIT_H
#define INIT_H
#include"structs.h"//already contains common.h
#include"highscore.h"
#include "SDL_image.h"
#include"SDL_mixer.h"
#include"background.h"
extern App app;// i tell it there is indeed an app somewhere. 
////Sets up the SDL enviroment
void initSDL();
void initGame();
#endif