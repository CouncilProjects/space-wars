#include"input.h"

void doInput()
{
    SDL_Event event;

    //take events as long as there are
    //for now only react to closing
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        
        default:
            break;
        }
    }
}