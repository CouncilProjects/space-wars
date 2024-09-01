#include"utility.h"

//Detects if the 2 rectangles (meaning the 2 assets) colided
int collision(int r1x,int r1y,int r1w,int r1h,int r2x,int r2y,int r2w,int r2h)
{
    return SDL_max(r1x,r2x) < SDL_min(r1x+r1w,r2x+r2w) && SDL_max(r1y,r2y) < SDL_min(r1y+r1h,r2y+r2h);
}

//Calculates which way the enemy ship needs to shoot
void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
    int steps = SDL_max(SDL_abs(x1 - x2), SDL_abs(y1 - y2));

    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2);
    *dx /= steps;

    *dy = (y1 - y2);
    *dy /= steps;
}