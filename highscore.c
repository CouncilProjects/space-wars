#include"highscore.h"

//returns the current highest score on the table
int getHighscore()
{
    return highscores.highscoresArray[0].score;
}

void initHighScoreTable()
{
    FILE *hsfile;
    if(hsfile=fopen("data.dat","rb"))//if there is a file with the highscores get them from there
    {
        fread(&highscores,sizeof(Highscore),8,hsfile);
        fclose(hsfile);
        newHighScore=NULL;
        cursorBlink=0;
        return;
    }

    //if this point is reached there is not previous table so we randomize one.
    int i;
    memset(&highscores,0,sizeof(Highscores));
    
    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        highscores.highscoresArray[i].score=(rand()%74)+13;
        STRNCPY(highscores.highscoresArray[i].name,"ANNONYMOUS",MAX_SCORE_NAME_LENGTH);
    }
    qsort(&highscores,NUM_HIGHSCORE,sizeof(Highscore),myCompare);
    newHighScore=NULL;
    cursorBlink=0;
}

void initHighScores()
{
    app.caller.logic=hsLogic;
    app.caller.draw=hsDraw;

    //clear the input so as to not immidiately change the screen. 
    memset(app.keys,0,MAX_KEYBOARD_KEYS);
}

static int r=200,b=0;
static void hsLogic()
{
    handleBackround();
    moveStarfield();

    //if newHighScore is not NULL (so its assigned) it means a player did a high score and we need to get his name
    if(newHighScore!=NULL)
    {
        handleNameInput();
    }
    else
    {
        if(app.keys[SDL_SCANCODE_SPACE])
        {
            resetStage();
        }
    }

    //this practiically resets the cursorBlink every second 
    if(++cursorBlink>=FPS)
    {
        cursorBlink=0;
    }

     if(b>=250)
    {
        b=0;
    }
    if(r<=5)
    {
        r=200;
    }
    b+=1;
    r-=1;
}

void saveTable()
{
    FILE *hsfile=fopen("data.dat","wb");
    if(hsfile==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"file","file not oppend",NULL);
    }
    fwrite(&highscores,sizeof(Highscore),8,hsfile);
    fclose(hsfile);
}

//Take the name the user types (its stored in app.playerInputText by the do input() function in input.c)
//turn it to uppercase letters (only uppercase support)
static void handleNameInput()
{
    int i,currentLen;
    char c;

    currentLen=strlen(newHighScore->name);

    for(i=0;i<strlen(app.playerInputText);i++)
    {
        c=toupper(app.playerInputText[i]);
        if(currentLen<MAX_SCORE_NAME_LENGTH&& (c>=' ' && c<='Z'))
        {
            newHighScore->name[currentLen++]=c;
        }
    }

    

    if(currentLen>0 && app.keys[SDL_SCANCODE_BACKSPACE])
    {
        newHighScore->name[--currentLen]='\0';
        app.keys[SDL_SCANCODE_BACKSPACE]=0;
    }

    if(app.keys[SDL_SCANCODE_RETURN])
    {
        if(strlen(newHighScore->name)==0)//empty name
        {
            STRNCPY(newHighScore->name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
        }

         newHighScore = NULL;
    }
    
}

static void hsDraw()
{
    drawBackround();
    drawStarfield();

    if(newHighScore!=NULL)
    {
        drawNameInputHUD();
    }
    else
    {
        drawHighscores();
    }
}

//Shows basic name entering UI, displays some text and the typed name so far, also a green rectangle that acts as a cursor
static void drawNameInputHUD()
{
    SDL_Rect rect;

    drawText(SCREEN_WIDTH/2,70,text_center,0,255,205,"CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE!");
    drawText(SCREEN_WIDTH/2,150,text_center,0,255,205,"ENTER YOUR NAME");
    drawText(SCREEN_WIDTH / 2, 250,text_center, 128, 255, 128,"%s", newHighScore->name);

    if(cursorBlink<=FPS/2)//every half a second practically it will make it appear
    {
        rect.y=250;
        rect.w=GLYPH_WIDTH;
        rect.h=GLYPH_HEIGHT;
        rect.x=((SCREEN_WIDTH/2)+(strlen(newHighScore->name)*GLYPH_WIDTH)/2)+5;

        SDL_SetRenderDrawColor(app.renderer,0,255,0,255);
        SDL_RenderFillRect(app.renderer,&rect);
    }

    drawText(SCREEN_WIDTH / 2, 625,text_center,r,SDL_abs(r-b),b, "PRESS ENTER WHEN FINISHED");

}
//formats the highscore table. 
static void drawHighscores()
{
    int i,x=425,y;
    y=70;
    drawText(x,70,text_left,255,255,255,"HIGHSCORE");

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
        int r=255,g=255,b=255;

        if(highscores.highscoresArray[i].recent==1)
        {
            b=0;
        }

        drawText(SCREEN_WIDTH/2,y,text_center,r,g,b,"#%d %-15s %03d",(i+1),highscores.highscoresArray[i].name,highscores.highscoresArray[i].score);

        y += 50;
    }

    drawText(x+30,550,text_left,r,SDL_abs(r-b),b,"PRESS SPACE TO PLAY");
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
   

    newHighScore=NULL;
    for(i=0;i<NUM_HIGHSCORE;i++)
    {
        highscores.highscoresArray[i]=newTable[i];
        if(highscores.highscoresArray[i].recent==1)
        {
            newHighScore=&highscores.highscoresArray[i];
            memset(newHighScore->name,'\0',sizeof(char)*16); //we also put null terminators here else it will be uninitialised at the start. 
        }
    }
}

//a compare function that decides if arg b is bigger than arg a
static int myCompare(const void *a,const void *b)
{
    Highscore *h1=((Highscore*)a);
    Highscore *h2=((Highscore*)b);

    return h2->score - h1->score;
}
