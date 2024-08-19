#include"stage.h"

extern App app;
extern Stage stage;
extern Entity *player;
SDL_Texture *bulletTexture,*enemyTexture;
int enemySpawnTime;

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

    //cache the textures for repeated use later.
    bulletTexture=loadTexture("textures/bullet.png");
    enemyTexture=loadTexture("textures/enemy.png");

    enemySpawnTime=0;
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
    player->side=PLAYER_SIDE;

    stage.fighterTail->next=player;
    stage.fighterTail=player;
    SDL_QueryTexture(player->texture,NULL,NULL,&player->w,&player->h);
}

static void logic(void)
{
    handlePlayer();
    handleEnemies();
    handleBullets();
    spawnEnemies();
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

}

static void handleEnemies()
{
    Entity *current,*prev;

    prev=&stage.fighterHead;

    //go through the list, move everyship and remove the non player ships that are off screen. 
    for(current=stage.fighterHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;

        //if its not a player and its either offscreen or its dead remove it. 
        if(current!=player && (current->x < -(current->w) || current->health==0))
        {

            
            if(stage.fighterTail==current)
            {
                stage.fighterTail=prev;
            }

            //if the current one is somewhere in the middle simple make the next of the previous show to the next of the current, then delete it.
            prev->next=current->next;
            free(current);
            current=prev;
        }

    }
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
    bullet->side=PLAYER_SIDE;

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
        
        if(current->x > SCREEN_WIDTH || bulletHit(current))
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

static int bulletHit(Entity *bullet)
{
    //we check each fighter to see if it collided. 
    Entity *current;

    for(current=stage.fighterHead.next;current!=NULL;current=current->next)
    {
        if(bullet->side!=current->side && bullet->health==1 && collision(bullet->x,bullet->y,bullet->w,bullet->h,current->x,current->y,current->w,current->h))
        {
            bullet->health=current->health=0;
            return 1;
        }
    }

    return 0;

}

static void spawnEnemies()
{
    enemySpawnTime--;

    if(enemySpawnTime>0)
    {
        return;
    }

    Entity *enemy;
    enemy=malloc(sizeof(Entity));
    memset(enemy,0,sizeof(Entity));

    stage.fighterTail->next=enemy;
    stage.fighterTail=enemy;
    enemy->x=SCREEN_WIDTH;
    enemy->y=rand()%SCREEN_HEIGHT;
    //negative dx so it move to the left of the screen. 
    enemy->dx=-(2+(rand()%5));
    enemy->dy=0;
    enemy->health=1;

    enemy->texture=enemyTexture;
    SDL_QueryTexture(enemy->texture,NULL,NULL,&enemy->w,&enemy->h);
    enemy->side=ALIEN_SIDE;
    courseCorrection(enemy);
    enemySpawnTime=30+(rand()%50);
}

static void courseCorrection(Entity *enemy)
{
    if(enemy->y+enemy->h > SCREEN_HEIGHT)
    {
        enemy->y-=(int)enemy->h/2;
    }
    else if(enemy->y+enemy->h < 0)
    {
        enemy->y+=(int)enemy->h/2;
    }
}

static void draw(void)
{
    drawShips();
    drawBullets();
}

static void drawShips(void)
{
    Entity *current;
    for(current=stage.fighterHead.next;current!=NULL;current=current->next)
    {
        drawTexture(current->texture,current->x,current->y);
    }
}

static void drawBullets(void)
{
    Entity *current;
    for(current=stage.bulletHead.next;current!=NULL;current=current->next)
    {
        drawTexture(current->texture,current->x,current->y);
    }
}
