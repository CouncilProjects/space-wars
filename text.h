#ifndef TEXT_H
#define TEXT_H
#include<stdarg.h> //for variable arguments
#include"common.h"
#include"draw.h"
#include"defs.h"

void initFont();
void drawText(int x,int y,int r,int g,int b,char *format,...);


#endif