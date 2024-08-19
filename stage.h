#ifndef STAGE_H
#define STAGE_H
#include"structs.h"
#include"draw.h"

void initStage(void);
static void initPlayer();
static void logic(void);
static void handlePlayer(void);
static void fire(void);
static void handleBullets(void);
static void draw(void);
static void drawPlayer(void);
static void drawBullets(void);
#endif