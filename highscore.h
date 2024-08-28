#ifndef HISHSCO_H
#define HIGHSCO_H

#include"structs.h"
#include"stage.h"
#include"background.h"
extern App app;
static Highscores highscores;
static Highscore *newHighScore;
static int cursorBlink;

//sets different high-score values
void initHighScoreTable();

//changes the logic and draw functions of the apps caller to take control. 
void initHighScores();

//Calls a series of functions needed for highscore logic [handleBackround(),moveStarfield(),initStage()]
static void hsLogic();

//Take the name the user types (its stored in app.playerInputText by the do input() function in input.c)
//turn it to uppercase letters (only uppercase support)
static void handleNameInput();

//Calls a series of functions needed for highscore logic [drawBackround(),drawStarfield(),drawHighscores()]
static void hsDraw();

//Shows basic name entering UI, displays some text and the typed name so far, also a green rectangle that acts as a cursor
static void drawNameInputHUD();

//Creates a rectangle and writes (with the drawText function of text.h) every score in the highscore array.
static void drawHighscores();

//We will make a new array with one extra score, the new one. 
//then we copy the old array in the new one.
//then we sort the array with qsort (will need a compare function) in decending order. 
//then put the sorted lowest idexed (0 to 7) of new Table to the old array.  
void addHighScore(int score);

//needed function of qsort. 
static int myCompare(const void *a,const void *b);

#endif