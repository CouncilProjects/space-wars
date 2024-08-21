#ifndef INPUT_H
#define INPUT_H
//This header has the function headers that deal with input.
#include"common.h"
#include"structs.h"

//take the incoming input events from the queque
void doInput();
//Notifies when a key is pressed  Parameter[a pointer to the triggering event]
void keyPressed(SDL_KeyboardEvent *event);

//Notifies when a key is released  Parameter[a pointer to the triggering event]
void keyReleased(SDL_KeyboardEvent *event);
#endif