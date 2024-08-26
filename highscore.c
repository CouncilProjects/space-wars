#include"highscore.h"


void initHighScoreTable()
{
    int i;
    memset(&highscores,0,sizeof(Highscores));
    
    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        highscores.highscoresArray[i].score=NUM_HIGHSCORE-i;
    }
}

void initHighScores()
{
    app.caller.logic=hsLogic;
    app.caller.draw=hsDraw;

    //clear the input so as to not immidiately change the screen. 
    memset(app.keys,0,sizeof(int)*MAX_KEYBOARD_KEYS);
}

static void hsLogic()
{
    handleBackround();
    moveStarfield();

    if(app.keys[SDL_SCANCODE_SPACE])
    {
        resetStage();
    }
}

static void hsDraw()
{
    drawBackround();
    drawStarfield();

    drawHighscores();
}

//formats the highscore table. 
static void drawHighscores()
{
    int i,x=425,y;
    y=70;
    drawText(x,70,255,255,255,"HIGHSCORE");

    SDL_Rect rect;
    rect.x=x-30;
    rect.y=100;
    rect.w=450;
    rect.h=600;
    SDL_SetRenderDrawColor(app.renderer,255,30,30,255);
    SDL_RenderDrawRect(app.renderer,&rect);
    SDL_SetRenderDrawColor(app.renderer,0,0,0,255);
    y+=50;
    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        if(highscores.highscoresArray[i].recent==1)
        {
            drawText(x,y,255,255,0,"#%d ............. %03d",(i+1),highscores.highscoresArray[i].score);
        }
        else
        {
            drawText(x,y,255,255,255,"#%d ............. %03d",(i+1),highscores.highscoresArray[i].score);
        }

        y += 50;
    }

    drawText(x,600,255,255,255,"PRESS SPACE TO PLAY");
}

//We will make a new array with one extra score, the new one. 
//then we copy the old array in the new one.
//then we sort the array with qsort (will need a compare function) in decending order. 
//then put the sorted lowest idexed (0 to 7) of new Table to the old array.  
void addHighScore(int score)
{
    int i;

    Highscore newTable[NUM_HIGHSCORE+1];

    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        newTable[i]=highscores.highscoresArray[i];
        newTable[i].recent=0;
    }

    newTable[NUM_HIGHSCORE].score=score;
    newTable[NUM_HIGHSCORE].recent=1;

    //due to myCompare the sort will happen in descending order (from biggest to lowest)
    qsort(newTable,NUM_HIGHSCORE+1,sizeof(Highscore),myCompare);
   
    int y=300;

    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        highscores.highscoresArray[i]=newTable[i];
    }
}

//a compare function that decides if arg b is bigger than arg a
static int myCompare(const void *a,const void *b)
{
    Highscore *h1=((Highscore*)a);
    Highscore *h2=((Highscore*)b);

    return h2->score - h1->score;
}
