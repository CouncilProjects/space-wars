#include"stage.h"

extern App app;
extern Stage stage;
extern Entity *player;
SDL_Texture *bulletTexture;
void initStage(void)
{
    //assign the functions to the apps function caller
    app.caller.logic=logic;
    app.caller.draw=draw;

    //set the memory of stage to zero
    memset(&stage,0,sizeof(stage));
    //set its pointers
    stage.fighterTail=&stage.fighterHead;
    stage.bulletTail=&stage.bulletHead;

    initPlayer();

    //cache the texture for repeated use later.
    bulletTexture=loadTexture("textures/bullet.png");
}

static void initPlayer()
{
    player=malloc(sizeof(Entity));
    if(player==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error making player (bad malloc)",NULL);
    }

    memset(player,0,sizeof(Entity));
    player->x=110;
    player->y=110;
    player->texture=loadTexture("textures/player.png");

    stage.fighterTail->next=player;
    stage.fighterTail=player;
    SDL_QueryTexture(player->texture,NULL,NULL,&player->w,&player->h);
}

static void logic(void)
{
    handlePlayer();
    handleBullets();
}

static void handlePlayer(void)
{
    player->dx=player->dy=0;

    if(player->reload>0)
    {
        player->reload--;
    }

    if(app.keys[SDL_SCANCODE_UP])
    {
        player->dy= -PLAYER_SPEED;
    }

     if(app.keys[SDL_SCANCODE_DOWN])
    {
        player->dy= PLAYER_SPEED;
    }

     if(app.keys[SDL_SCANCODE_LEFT])
    {
        player->dx= -PLAYER_SPEED;
    }

    if(app.keys[SDL_SCANCODE_RIGHT])
    {
        player->dx= PLAYER_SPEED;
    }

    if(app.keys[SDL_SCANCODE_SPACE] && player->reload==0)
    {
        fire();
    }

    player->x += player->dx;
    player->y += player->dy;
}

static void fire(void)
{
    
    Entity *bullet;

    bullet=malloc(sizeof(Entity));
    memset(bullet,0,sizeof(Entity));
    if(bullet==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error making bullet (bad malloc)",NULL);
    }

    //insert new bullet in the list 
    stage.bulletTail->next=bullet;
    stage.bulletTail=bullet;

    bullet->x=player->x;
    bullet->y=player->y;
    bullet->dx=PLAYER_BULLET_SPEED;
    bullet->dy=0;
    bullet->health=1;
    bullet->texture=bulletTexture;
    SDL_QueryTexture(bullet->texture,NULL,NULL,&bullet->w,&bullet->h);

    bullet->y= player->y+(player->h/2)-(bullet->h/2);
    

    //tell it that 8 frames must pass before firing again. 
    player->reload=8;
}

static void handleBullets(void)
{
    Entity *current,*prev;

    prev=&stage.bulletHead;

    for(current=stage.bulletHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        
        if(current->x > SCREEN_WIDTH)
        {

            //if the current one is at the end of the list make the end of the lists its previous one.
            if(current == stage.bulletTail)
            {
                stage.bulletTail=prev;
            }

            //if the current one is somewhere in the middle simple make the next of the previous show to the next of the current, then delete it.
            prev->next=current->next;
            free(current);
            current=prev;
        }

    }
}

static void draw(void)
{
    drawPlayer();
    drawBullets();
}

static void drawPlayer(void)
{
    drawTexture(player->texture,player->x,player->y);
}

static void drawBullets(void)
{
    Entity *current;
    for(current=stage.bulletHead.next;current!=NULL;current=current->next)
    {
        drawTexture(current->texture,current->x,current->y);
    }
}