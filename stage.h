#ifndef STAGE_H
#define STAGE_H
#include"structs.h"
#include"draw.h"
#include"utility.h"
#include"sound.h"
#include"text.h"
#include"highscore.h"
#include"background.h"

//Initializes basic resources [fighter & bullet lists, player,textures,app function callers]
void initStage(void);

//Resets the screen after player has been destroyed
void resetStage(void);

//Initializes a player ship. 
static void initPlayer(void);

//Calls the series of functions that control the flow of the game [controlPlayer,handleShips,enemiesShoot,handleBullets,spawnEnemies,clipPlayer,resetStage]
static void logic(void);

//moves the points and handles collection
static void handlePoints();

//If a boss is not present create one (Boss is an Entity pointed at by activeBoss pointer)
static void addBoss();

// Makes enemies unable to spawn off-screen
static void courseCorrection(Entity *enemy);

//Controls player actions based on user input
static void controlPlayer(void);

//Player specific firing mechanism
static void fire(void);

//Moves the destroyed texture adding explosions and debri when it ends
static void moveDestroyed();

//Creates a series of explosions in different colors
static void addExplosion(int x,int y,int num);

//cuts the texture to 4 parts adding each one as a "debri"
static void addDebris(Entity *en);

// Moves every ship on screen (inside the fighter list) removing those that are off-screen or destroyed
static void handleShips(void);

//Creates a point bubble
static void addPoint(int x,int y);

//Adds a new destryed animation to be tracked
static void addDestroyed(Entity *en);

//Orders every enemy ship with available projectile to shoot.
static void enemiesShoot(void);

//Enemy specific firing mechanism
static void fireAlienBullet(Entity *e);

//If there is an active boss move it. changing its direction if it tries to go above or below the screen
static void moveBoss();

//create a new destroyed animation sequence, and mark it as a boss death by placing the destructionSequenceBoss pointer
static void bossDeath();

//a Boss-specific firing mechanism fires faster and bullets are also faster
static void bossFire();

//Moves every bullet existing on the map. removing those out of the screen or those that collided
static void handleBullets(void);

//Checks if the bullet has collided with any ships.
static int bulletHit(Entity *bullet);

//Periodically make a new enemy and add it to the fighter list.
static void spawnEnemies(void);

//Makes the player unable to go beyond the screen
static void clipPlayer(void);

//moves the explosions, removing them if they surpass their lifetimes
static void handleExplosion();

//moves the debri, removing them if they surpass their lifetimes
static void handleDebris(void);

//Calls the series of functions that control the graphics of the game [drawShips,drawBullets]
static void draw(void);

//draws every point in the list
static void drawPoints();

//If there is a boss present it will be drawn. 
static void drawBoss();

//draws the destroy animation for each ship
static void drawDestroyed();

//Draws every ship on screen with updated coordinates
static void drawShips(void);

//Draws every bullet on screen with updated coordinates
static void drawBullets(void);

//goes through the list drawing every debri
static void drawDebris(void);

//goes through the list drawing every explosion
static void drawExplosions(void);

//Displayes the text for score and highscore
static void drawHUD();
#endif