#include"input.h"

extern App app;


//take the incoming input events from the queque
void doInput()
{
    SDL_Event event;

    //take events as long as there are
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        
        case SDL_KEYDOWN:
            keyPressed(&event.key);
            break;
        case SDL_KEYUP:
            keyReleased(&event.key);
            break;
        default:
            break;
        }
    }
}


//This events such ass keyPressed or keyReleased ocure only once.Because we want continious movement 
//we use the app variables that simply stay 1 as long as its not released insted of pressing it many times

//Notifies when a key is pressed Parameter[a pointer to the triggering event]
void keyPressed(SDL_KeyboardEvent *event)
{
    //we will only handle the event if its the first time it comes, ignoring repeats
    if(event->repeat==0 && event->keysym.scancode<MAX_KEYBOARD_KEYS)
    {
        //Reminder that (*event).keysym.scancode == event->keysym.scancode
        // keysym: is the key from which the event came, scancode is the hardware assosiated code. 
        //Now the scancode will mark its corresponding sppot in the array as 1
        app.keys[event->keysym.scancode]=1;
    }
}

//Notifies when a key is released Parameter[a pointer to the triggering event]
void keyReleased(SDL_KeyboardEvent *event)
{
    //we will only handle the event if its the first time it comes, ignoring repeats
    if(event->repeat==0 && event->keysym.scancode<MAX_KEYBOARD_KEYS)
    {
        //Reminder that (*event).keysym.scancode == event->keysym.scancode
        // keysym: is the key from which the event came, scancode is the hardware assosiated code. 
        //Now the scancode will mark its corresponding sppot in the array as 1
        app.keys[event->keysym.scancode]=0;
    }
}