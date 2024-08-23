#include"stage.h"

extern App app;
extern Stage stage;
extern Entity *player;
SDL_Texture *bulletTexture,*enemyTexture,*alienBulletTexture,*playerTexture,*explosionTexture,*backgroundTexture;
int enemySpawnTime,stageResetTimer;
int backroundX;
static Star stars[MAX_STARS];

//Initializes basic resources [fighter & bullet lists, player,textures,app function callers]
void initStage(void)
{
    //assign the functions to the apps function caller
    app.caller.logic=logic;
    app.caller.draw=draw;

    //set the memory of stage to zero
    memset(&stage,0,sizeof(stage));
    //set the pointers of the lists with assets
    stage.fighterTail=&stage.fighterHead;
    stage.bulletTail=&stage.bulletHead;
    stage.explosionTail=&stage.explosionHead;
    stage.debrisTail=&stage.debrisHead;


    //cache the textures for repeated use later.
    bulletTexture=loadTexture("textures/bullet.png");
    enemyTexture=loadTexture("textures/enemy.png");
    alienBulletTexture = loadTexture("textures/alienBullet.png");
    playerTexture = loadTexture("textures/player.png");
    explosionTexture=loadTexture("textures/explosion.png");
    backgroundTexture=loadTexture("textures/background.png");
    if(backgroundTexture==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"error","Background texture did not load",NULL);
    }


    resetStage();
    enemySpawnTime=0;
}

//Resets the screen after player has been destroyed
static void resetStage(void)
{
    Entity *e;
    Explosion *ex;
    Debris *de;

    //empty the fighter list
    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }
     //empty the bullet list
    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    //empty the debri list
    while (stage.debrisHead.next)
    {
        de=stage.debrisHead.next;
        stage.debrisHead.next=de->next;
        free(de);
    }
    //empty the explosion list
    while (stage.explosionHead.next)
    {
        ex=stage.explosionHead.next;
        stage.explosionHead.next=ex->next;
        free(ex);
    }
    

    memset(&stage, 0, sizeof(Stage));
    //restart the list tails
    stage.fighterTail=&stage.fighterHead;
    stage.bulletTail=&stage.bulletHead;
    stage.explosionTail=&stage.explosionHead;
    stage.debrisTail=&stage.debrisHead;


    //make starfield
    initStarfield();

    //make new player
    initPlayer();


    enemySpawnTime = 10;

    stageResetTimer = FPS * 4;
}

//Sets the star array with rando star positions
static void initStarfield()
{
    int i;
    for(i=0;i<MAX_STARS;i++)
    {
        stars[i].x=rand()%SCREEN_WIDTH;
        stars[i].y=rand()%SCREEN_HEIGHT;
        stars[i].speed=1+(rand()%8);
    }
}

//Initializes a player ship. 
static void initPlayer()
{
    player=malloc(sizeof(Entity));
    if(player==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Error making player",NULL);
    }

    memset(player,0,sizeof(Entity));
    player->x=110;
    player->y=110;
    player->health=1;
    player->texture=playerTexture;
    SDL_QueryTexture(player->texture,NULL,NULL,&player->w,&player->h);
    player->side=PLAYER_SIDE;
    
    //place player ship in the list
    stage.fighterTail->next=player;
    stage.fighterTail=player;
}

//Calls the series of functions that control the flow of the game [controlPlayer,handleShips,enemiesShoot,handleBullets,spawnEnemies,clipPlayer,resetStage]
static void logic(void)
{
    handleBackround();
    moveStarfield();
    controlPlayer();
    handleShips();
    enemiesShoot();
    handleBullets();
    spawnEnemies();
    clipPlayer();
    handleExplosion();
    handleDebris();
    if (player == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"You died","Press OK to restart",NULL);
        resetStage();
    }
}

//makes sure the backround is always in place. 
static void  handleBackround()
{
    if(--backroundX < -SCREEN_WIDTH)
    {
        backroundX=0;
    }
}

//moves every star in the static array, warping them when they reach the end of the screen.
static void  moveStarfield()
{
    int i;
    for(i=0;i<MAX_STARS;i++)
    {
        stars[i].x-=stars[i].speed;

        if(stars[i].x < 0)
        {
            stars[i].x=SCREEN_WIDTH+stars[i].x;
        }
    }
}
//Controls player actions based on user input
static void controlPlayer(void)
{

    if(player==NULL)
    {
        return;
    }

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
        playSound(SND_PLAYER_FIRE,ch_player);
        fire();
    }

}

// Moves every ship on screen (inside the fighter list) removing those that are off-screen or destroyed
static void handleShips()
{
    Entity *current,*prev;

    prev=&stage.fighterHead;

    if(player==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"The is no player in handle enemy","I have no idea why",NULL);
    }
    //go through the list, move everyship and remove the non player ships that are off screen. 
    for(current=stage.fighterHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;

        current->x += current->dx;
        current->y += current->dy;

        if (current != player && current->x < -current->w)
        {
            current->health = 0;
        }

        if (current->health == 0)
        {
            if (current == player)
            {
                playSound(SND_PLAYER_DIE,ch_player);
                addDebris(current);
                player = NULL;
            }

            if (current == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            //maybe
            if (current != player && current->x >0)
            {
                playSound(SND_ALIEN_DIE,ch_any);
                addDebris(current);
                addExplosion(current->x,current->y,3);
            }
            prev->next = current->next;
            free(current);
            current = prev;
        }

    }
}

//Creates a series of explosions in different colors
static void addExplosion(int x,int y,int num)
{
    Explosion *ex;
    int i;
    for(i=0;i<num;i++)
    {
        ex=malloc(sizeof(Explosion));
        memset(ex,0,sizeof(Explosion));

        stage.explosionTail->next=ex;
        stage.explosionTail=ex;

        ex->x = x + (rand() % 32) - (rand() % 32);
        ex->y = y + (rand() % 32) - (rand() % 32);
        ex->dx = (rand() % 10) - (rand() % 10);
        ex->dy = (rand() % 10) - (rand() % 10);

        ex->dx /= 10;
        ex->dy /= 10;

        //we queque up many explotions to make colors
        switch (rand()%4)
        {
        case 0:
            ex->r=255;
            break;
        case 1:
            ex->r=255;
            ex->g=128;
            break;
        case 2:
            ex->r=255;
            ex->g=255;
            break;
        
        default:
            ex->r = 255;
            ex->g = 255;
            ex->b = 255;
            break;
        }
        //a is its time to live
        ex->a = rand() % FPS * 3;
    }
}

//cuts the texture to 4 parts adding each one as a "debri"
static void addDebris(Entity *en)
{
    Debris *d;
    int x, y, w, h;
    //we devide by 2 then with a double for it will split the texture to 4 parts. 
    w=en->w/2;
    h=en->h/2;

    for(y=0;y<en->h;y+=h)
    {
        for(x=0;x<en->w;x+=w)
        {
            d=malloc(sizeof(Debris));
            memset(d,0,sizeof(Debris));

            d->x=en->x+w;
            d->y=en->y+h;

            d->dx=(rand()%5)-(rand()%5);
            d->dy=-(5+rand()%12);
            d->life=FPS*2;
            d->texture=en->texture;
            if(d->texture==NULL)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"error","No debri texture",NULL);
            }

            // what part of the texture it covers. 
            d->rect.x=x;
            d->rect.y=y;
            d->rect.w=w;
            d->rect.h=h;

            stage.debrisTail->next=d;
            stage.debrisTail=d;

        }
    }
}
//Orders every enemy ship with available projectile to shoot.
static void enemiesShoot(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            playSound(SND_ALIEN_FIRE,ch_alien_fire);
            fireAlienBullet(e);
        }
    }
}

//Enemy specific firing mechanism
static void fireAlienBullet(Entity *e)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = ALIEN_SIDE;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x +=e->x+ (e->w / 2) - (bullet->w / 2);
    bullet->y +=e->y+ (e->h / 2) - (bullet->h / 2);
    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED+e->dx;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS * 2);
}

//Player specific firing mechanism
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

//Moves every bullet existing on the map. removing those out of the screen or those that collided
static void handleBullets(void)
{
    Entity *current,*prev;

    prev=&stage.bulletHead;

    for(current=stage.bulletHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        
        if(bulletHit(current) || current->x < -current->w || current->y < -current->h || current->x > SCREEN_WIDTH || current->y > SCREEN_HEIGHT)
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

//Checks if the bullet has collided with any ships.
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

//Periodically make a new enemy and add it to the fighter list.
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
    enemy->reload = FPS * (1 + (rand() % 3));
    enemySpawnTime=30+(rand()%50);
}

//Makes the player unable to go beyond the screen
static void clipPlayer(void)
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }
        
        if (player->x > SCREEN_WIDTH-player->w)
        {
            player->x = SCREEN_WIDTH-player->w;
        }

        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}

//moves the explosions, removing them if they surpass their lifetime
static void handleExplosion()
{
    Explosion *current,*prev;
    prev=&stage.explosionHead;

    for(current=stage.explosionHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;

        if(--current->a < 0)
        {
            if(current==stage.explosionTail)
            {
                stage.explosionTail=prev;
            }

            prev->next=current->next;
            free(current);
            current=prev;
        }
    }
    
}

//moves the debri, removing them if they surpass their lifetime
static void handleDebris()
{
    Debris *current,*prev;
    prev=&stage.debrisHead;

    for(current=stage.debrisHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;
        current->dy+=0.5;
        if(--current->life < 0)
        {
            if(current==stage.debrisTail)
            {
                stage.debrisTail=prev;
            }

            prev->next=current->next;
            free(current);
            current=prev;
        }
    }
    
}

// Makes enemies unable to spawn off-screen
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

//Calls the series of functions that control the graphics of the game [drawShips,drawBullets]
static void draw(void)
{
    drawBackround();
    drawStarfield();

    drawShips();
    drawBullets();
    drawExplosions();

    drawDebris();
}

//Sets the background to the size of the screen
static void drawBackround()
{
    SDL_Rect rect;
    int x;
    for(x=0;x<SCREEN_WIDTH;x+=SCREEN_WIDTH)
    {
        rect.x=x;
        rect.y=0;
        rect.h=SCREEN_HEIGHT;
        rect.w=SCREEN_WIDTH;

        SDL_RenderCopy(app.renderer,backgroundTexture,NULL,&rect);
    }
}

//set the color of the stars and draw them as lines
static void drawStarfield(void)
{
    int i,c;
    for(i=0;i<MAX_STARS;i++)
    {
        c=32*stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer,c,c,c,255);

        SDL_RenderDrawLine(app.renderer,stars[i].x,stars[i].y,stars[i].x+3,stars[i].y);
    }
}

//Draws every ship on screen with updated coordinates
static void drawShips(void)
{
    Entity *current;
    for(current=stage.fighterHead.next;current!=NULL;current=current->next)
    {
        drawTexture(current->texture,current->x,current->y);
    }
}

//Draws every bullet on screen with updated coordinates
static void drawBullets(void)
{
    Entity *current;
    for(current=stage.bulletHead.next;current!=NULL;current=current->next)
    {
        drawTexture(current->texture,current->x,current->y);
    }
}

//goes through the list drawing every debri
static void drawDebris()
{
    Debris *current;
    for(current=stage.debrisHead.next;current!=NULL;current=current->next)
    {
        drawPartialTexture(current->texture,&current->rect,current->x,current->y);
    }
}


//goes through the list drawing every explosion
static void drawExplosions()
{
    Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer,SDL_BLENDMODE_ADD);
    //Enable additive blending, when adding textures one upon the other they blend colours
    SDL_SetTextureBlendMode(explosionTexture,SDL_BLENDMODE_ADD);

    for(e=stage.explosionHead.next;e!=NULL;e=e->next)
    {
        SDL_SetTextureColorMod(explosionTexture,e->r,e->g,e->b);
        SDL_SetTextureAlphaMod(explosionTexture,e->a);

        drawTexture(explosionTexture,e->x,e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);


}