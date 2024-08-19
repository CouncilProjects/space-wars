#include"utility.h"
int collision(int r1x,int r1y,int r1w,int r1h,int r2x,int r2y,int r2w,int r2h)
{
    return SDL_max(r1x,r2x) < SDL_min(r1x+r1w,r2x+r2w) && SDL_max(r1y,r2y) < SDL_min(r1y+r1h,r2y+r2h);
}