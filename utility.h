#ifndef UTIL_H
#define UTIL_H
#include"common.h"

int collision(int r1x,int r1y,int r1w,int r1h,int r2x,int r2y,int r2w,int r2h);
void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy);
#endif