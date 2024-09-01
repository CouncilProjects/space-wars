#ifndef TEXT_H
#define TEXT_H
#include<stdarg.h> //for variable arguments
#include"common.h" //also has defs.h
#include"draw.h"
#include <stdio.h> //for vsprintf

void initFont();
void drawText(int x,int y,int align,int r,int g,int b,char *format,...);


#endif