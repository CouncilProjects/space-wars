#ifndef STAGE_H
#define STAGE_H
#include"structs.h"
#include"draw.h"
#include"utility.h"
void initStage(void);
static void initPlayer();
static void logic(void);
static void handlePlayer(void);
static void fire(void);
static void handleEnemies(void);
static void handleBullets(void);
static int bulletHit(Entity *bullet);
static void spawnEnemies(void);
static void courseCorrection(Entity *enemy);
static void draw(void);
static void drawShips(void);
static void drawBullets(void);
#endif