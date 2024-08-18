#ifndef INPUT_H
#define INPUT_H
//This header has the function headers that deal with input.
#include"common.h"
#include"structs.h"
void doInput();
//Handle when a key is pressed or released, pass a pointer to the triggering event
void keyPressed(SDL_KeyboardEvent *event);
void keyReleased(SDL_KeyboardEvent *event);
#endif