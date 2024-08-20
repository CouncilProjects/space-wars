#ifndef STAGE_H
#define STAGE_H
#include"structs.h"
#include"draw.h"
#include"utility.h"

//Initializes basic resources [fighter & bullet lists, player,textures,app function callers]
void initStage(void);

//Resets the screen after player has been destroyed
static void resetStage(void);

//Initializes a player ship. 
static void initPlayer(void);

//Calls the series of functions that control the flow of the game [controlPlayer,handleShips,enemiesShoot,handleBullets,spawnEnemies,clipPlayer,resetStage]
static void logic(void);

//Controls player actions based on user input
static void controlPlayer(void);

// Moves every ship on screen (inside the fighter list) removing those that are off-screen or destroyed
static void handleShips(void);

//Orders every enemy ship with available projectile to shoot.
static void enemiesShoot(void);

//Enemy specific firing mechanism
static void fireAlienBullet(Entity *e);

//Player specific firing mechanism
static void fire(void);

//Moves every bullet existing on the map. removing those out of the screen or those that collided
static void handleBullets(void);

//Checks if the bullet has collided with any ships.
static int bulletHit(Entity *bullet);

//Periodically make a new enemy and add it to the fighter list.
static void spawnEnemies(void);

//Makes the player unable to go beyond the screen
static void clipPlayer(void);

// Makes enemies unable to spawn off-screen
static void courseCorrection(Entity *enemy);

//Calls the series of functions that control the graphics of the game [drawShips,drawBullets]
static void draw(void);

//Draws every ship on screen with updated coordinates
static void drawShips(void);

//Draws every bullet on screen with updated coordinates
static void drawBullets(void);
#endif