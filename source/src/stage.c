#include"stage.h"

extern App app;
extern Stage stage;
extern Entity *player;
Entity *activeBoss,*destructionSequenceBoss;
int enemySpawnTime,stageResetTimer,warningBlink;

//Initializes basic resources [fighter,bullet,debri,explotion,point lists, player,textures,app function callers,boss markers]
void initStage(void)
{

    //set the memory of stage to zero
    memset(&stage,0,sizeof(stage));
    //set the pointers of the lists with assets
    stage.fighterTail=&stage.fighterHead;
    stage.bulletTail=&stage.bulletHead;
    stage.explosionTail=&stage.explosionHead;
    stage.debrisTail=&stage.debrisHead;
    stage.pointTail=&stage.pointHead;

    stage.destroyedTail=&stage.destroyedHead;

    activeBoss=NULL;
    destructionSequenceBoss=NULL;
    app.highScore=0;;
    resetStage();
}

//Resets the screen after player has been destroyed
void resetStage(void)
{
    Entity *e;
    Explosion *ex;
    Debris *de;

    //assign the functions to the apps function caller
    app.caller.logic=logic;
    app.caller.draw=draw;

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

    //empty the point list
    while (stage.pointHead.next)
    {
        e=stage.pointHead.next;
        stage.pointHead.next=e->next;
        free(e);
    }

    //empty the death animation list
    while (stage.destroyedHead.next)
    {
        e = stage.destroyedHead.next;
        stage.destroyedHead.next = e->next;
        free(e);
    }
    

    memset(&stage, 0, sizeof(Stage));
    //restart the list tails
    stage.fighterTail=&stage.fighterHead;
    stage.bulletTail=&stage.bulletHead;
    stage.explosionTail=&stage.explosionHead;
    stage.debrisTail=&stage.debrisHead;
    stage.pointTail=&stage.pointHead;
    stage.destroyedTail=&stage.destroyedHead;

    if(activeBoss!=NULL)
    {
        free(activeBoss);
    }
    if(destructionSequenceBoss!=NULL)
    {
        free(destructionSequenceBoss);
    }
    activeBoss=NULL;
    destructionSequenceBoss=NULL;

    //reset score
    stage.score=0;

    app.highScore=getHighscore();
    //make new player
    initPlayer();


    enemySpawnTime = 10;

    stageResetTimer = FPS * 3;
}

//Initializes a player ship. 
static void initPlayer()
{
    player=malloc(sizeof(Entity));

    memset(player,0,sizeof(Entity));
    player->x=110;
    player->y=110;
    player->health=10;
    
    player->texture=loadTexture("textures/player.png");
    SDL_QueryTexture(player->texture,NULL,NULL,&player->w,&player->h);
    player->side=PLAYER_SIDE;
    
    //place player ship in the list
    stage.fighterTail->next=player;
    stage.fighterTail=player;

}

//Calls the series of functions that control the flow of the game
static void logic(void)
{
    handleBackround(); //background.h
    moveStarfield(); //background.h
    handlePoints();
    controlPlayer();
    moveDestroyed();
    handleShips();
    enemiesShoot();
    moveBoss();
    bossFire();
    handleBullets();
    spawnEnemies();
    clipPlayer();
    handleExplosion();
    handleDebris();
    if (player == NULL && --stageResetTimer==0)
    {
        addHighScore(stage.score);
        initHighScores();
    }
}

//moves the points and handles collection
static void handlePoints()
{
    Entity *current,*prev;
    prev=&stage.pointHead;
    for(current=stage.pointHead.next;current!=NULL;current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;
        //if its outside of screen bring it back
        if(current->x < 0 || current->x+current->w > SCREEN_WIDTH)
        {
            current->dx*=-1;
            current->x+=current->dx;
        }

        if(current->y < 0 || current->y+current->h > SCREEN_HEIGHT)
        {
            current->dy*=-1;
            current->y+=current->dy;
        }

        if(player!=NULL &&  collision(current->x,current->y,current->w,current->h,player->x,player->y,player->w,player->h))
        {
            current->health=0; //caught
            stage.score++;
            app.highScore=SDL_max(stage.score,app.highScore);
            playSound(SND_POINT,ch_point);

            if(stage.score%25==0) //every 25 points that the player collects call addBoss()
            {
                addBoss();
            }
            
        }

        if(--current->health<=0)
        {
            if(current==stage.pointTail)
            {
                stage.pointTail=prev;
            }

            prev->next=current->next;
            free(current);
            current=prev;
        }
        prev=current;

    }
}

//If a boss is not present create one (Boss is an Entity pointed at by activeBoss pointer)
void addBoss()
{
    if(activeBoss!=NULL)
    {
        return;
    }
    activeBoss=malloc(sizeof(Entity));
    memset(activeBoss,0,sizeof(Entity));
    
    activeBoss->texture=loadTexture("textures/enemy.png");
    SDL_QueryTexture(activeBoss->texture,NULL,NULL,&activeBoss->w,&activeBoss->h);
    activeBoss->w+=100;
    activeBoss->h+=100;
    activeBoss->x=SCREEN_WIDTH+(activeBoss->w*2);
    activeBoss->y=SCREEN_HEIGHT/2+(-300+(rand()%600));
    courseCorrection(activeBoss);

    activeBoss->dx=rand()%2+1;
    activeBoss->dy=rand()%3+1;
    activeBoss->health=(stage.score/20)*8 +1;
    activeBoss->reload=0;

    activeBoss->side=ALIEN_SIDE;
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

    if(app.keys[SDL_SCANCODE_UP] || app.keys[SDL_SCANCODE_W])
    {
        player->dy= -PLAYER_SPEED;
    }

     if(app.keys[SDL_SCANCODE_DOWN] || app.keys[SDL_SCANCODE_S])
    {
        player->dy= PLAYER_SPEED;
    }

     if(app.keys[SDL_SCANCODE_LEFT] || app.keys[SDL_SCANCODE_A])
    {
        player->dx= -PLAYER_SPEED;
    }

    if(app.keys[SDL_SCANCODE_RIGHT] || app.keys[SDL_SCANCODE_D])
    {
        player->dx= PLAYER_SPEED;
    }

    if(app.keys[SDL_SCANCODE_SPACE] && player->reload==0)
    {
        playSound(SND_PLAYER_FIRE,ch_player);
        fire();
    }

}

//Player specific firing mechanism
static void fire(void)
{
    
    Entity *bullet;

    bullet=malloc(sizeof(Entity));
    memset(bullet,0,sizeof(Entity));

    //insert new bullet in the list 
    stage.bulletTail->next=bullet;
    stage.bulletTail=bullet;

    bullet->x=player->x;
    bullet->y=player->y;
    bullet->dx=PLAYER_BULLET_SPEED;
    bullet->dy=0;
    bullet->health=1;
    
    bullet->texture=loadTexture("textures/bullet.png");
    SDL_QueryTexture(bullet->texture,NULL,NULL,&bullet->w,&bullet->h);
    bullet->side=PLAYER_SIDE;

    bullet->y= player->y+(player->h/2)-(bullet->h/2);
    

    //tell it that 8 frames must pass before firing again. 
    player->reload=8;
}

//Moves the destroyed texture adding explosions and debri when it ends
static void moveDestroyed()
{
    Entity *current,*prev;
    prev=&stage.destroyedHead;
    for(current=stage.destroyedHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;
        current->health--;

        if(current==destructionSequenceBoss && current->health%12==0)
        {
            playSound(SND_ALIEN_DIE,ch_any);
        }

        if(current->health<=0)
        {
            if(current==stage.destroyedTail)
            {
                stage.destroyedTail=prev;
            }
            
            if(current==destructionSequenceBoss)
            {
                destructionSequenceBoss=NULL;
            }
            playSound(SND_ALIEN_DIE,ch_any);
            addExplosion(current->x+41,current->y+50,4);
            addDebris(current);
            prev->next=current->next;
            free(current);
            current=prev;
            
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

// Moves every ship on screen (inside the fighter list) removing those that are off-screen or destroyed
static void handleShips()
{
    Entity *current,*prev;

    prev=&stage.fighterHead;
    //go through the list, move everyship and remove the non player ships that are off screen. 
    for(current=stage.fighterHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;

        if (current != player && current->x < -current->w)
        {
            current->health = 0;
        }

        if (current->health == 0)
        {
            if (current == player)
            {
                playSound(SND_PLAYER_DIE,ch_player);
                addExplosion(current->x,current->y,10);
                addDebris(current);
                player = NULL;
            }
            else if(current->x>0)//stuff to do when enemy dies on screen
            {
                addPoint(current->x+(current->w/2),current->y+(current->h/2));
                addDestroyed(current);
            }


            if (current == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = current->next;
            free(current);
            current = prev;
        }

    }
}

//Creates a point bubble
static void addPoint(int x,int y)
{
    Entity *point=malloc(sizeof(Entity));
    memset(point,0,sizeof(Entity));

    point->x=x;
    point->y=y;
    point->dx=(rand()%5-rand()%5);
    point->dy=(rand()%3-rand()%5);

    point->health=FPS*10; //lives for 10 seconds


    point->texture=loadTexture("textures/points.png");
    SDL_QueryTexture(point->texture,NULL,NULL,&point->w,&point->h);

    stage.pointTail->next=point;
    stage.pointTail=point;
}

//Adds a new destryed animation to be tracked
static void addDestroyed(Entity *en)
{
    Entity *newDestroyed=malloc(sizeof(Entity));
    memset(newDestroyed,0,sizeof(Entity));

    newDestroyed->x=en->x-41;
    newDestroyed->y=en->y-50;
    newDestroyed->dx=en->dx;
    newDestroyed->dy=0;
    newDestroyed->health=FPS*3; //it needs to go through 15 phases in 3 seconds 

    
    newDestroyed->texture=loadTexture("textures/destroy.png");
    SDL_QueryTexture(newDestroyed->texture,NULL,NULL,&newDestroyed->w,&newDestroyed->h);
    newDestroyed->w/=15;

    stage.destroyedTail->next=newDestroyed;
    stage.destroyedTail=newDestroyed;
}

//Orders every non-boss enemy ship with available projectile to shoot.
static void enemiesShoot(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0 && (e->x+e->w)/2>0 && (e->x+e->w)/2<SCREEN_WIDTH && (e->y+e->h)/2>0 && (e->x+e->w)/2<SCREEN_HEIGHT)
        {
            playSound(SND_ALIEN_FIRE,ch_alien_fire);
            fireAlienBullet(e);
        }
    }
}

//Common enemy specific firing mechanism
static void fireAlienBullet(Entity *striker)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->health = 1;
    
    bullet->texture = loadTexture("textures/alienBullet.png");
    bullet->side = ALIEN_SIDE;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x +=striker->x+ (striker->w / 2) - (bullet->w / 2);
    bullet->y +=striker->y+ (striker->h / 2) - (bullet->h / 2);
    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), striker->x+(striker->w/2), striker->y+(striker->h/2), &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    striker->reload = (rand() % FPS * 3);
}


//If there is an active boss move it. changing its direction if it tries to go above or below the screen
static void moveBoss()
{
    if(activeBoss==NULL)
    {
        return;
    }

    activeBoss->x-=activeBoss->dx;
    activeBoss->y-=activeBoss->dy;
    if(activeBoss->y<0 || activeBoss->y+activeBoss->h>SCREEN_HEIGHT)
    {
        activeBoss->dy*=-1;
    }

    if(activeBoss->health<=0) //if it is destroyed on screen, call its destruction sequence and free resources
    {
        bossDeath();
        free(activeBoss);
        activeBoss=NULL;
        return;
    }
    else if(activeBoss->x+activeBoss->w<0) //if off screen simple free resources
    {
        free(activeBoss);
        activeBoss=NULL;
        return;
    }
}

//create a new destroyed animation sequence, and mark it as a boss death by placing the destructionSequenceBoss pointer
static void bossDeath()
{
    Entity *newDestroyed=malloc(sizeof(Entity));
    memset(newDestroyed,0,sizeof(Entity));

    destructionSequenceBoss=newDestroyed;
    stage.destroyedTail->next=newDestroyed;
    stage.destroyedTail=newDestroyed;

    
    newDestroyed->texture=loadTexture("textures/destroy.png");
    SDL_QueryTexture(newDestroyed->texture,NULL,NULL,&newDestroyed->w,&newDestroyed->h);

    newDestroyed->w/=15; //there are 15 phases. 

    newDestroyed->x=activeBoss->x-41;//due to the difference in size compared with the normal texture
    newDestroyed->y=activeBoss->y-50;

    newDestroyed->dx=-activeBoss->dx;

    for(int i=0;i<10;i++) //award 10 points for victory
    {
        addPoint(activeBoss->x+(-4+rand()%8),activeBoss->y+(-4+rand()%8));
    }

    newDestroyed->dy+=2; //will start "going down"
    newDestroyed->health=FPS*3; //will be on screen for 3 seconds before fully exploding
}

//a Boss-specific firing mechanism fires faster and bullets are also faster
static void bossFire()
{
    if(activeBoss==NULL || player==NULL|| --activeBoss->reload>0)
    {
        return;
    }
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->health = 1;
    
    bullet->texture = loadTexture("textures/alienBullet.png");
    bullet->side = ALIEN_SIDE;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x +=activeBoss->x+ (activeBoss->w / 2) - (bullet->w / 2);
    bullet->y +=activeBoss->y+ (activeBoss->h / 2) - (bullet->h / 2);
    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), activeBoss->x+(activeBoss->w/2), activeBoss->y+(activeBoss->h/2), &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED*2;
    bullet->dy *= ALIEN_BULLET_SPEED*2;

    activeBoss->reload = (rand() % FPS * 2/3);
}

//Moves every bullet existing on the map. removing those out of the screen or those that collided
static void handleBullets(void)
{
    Entity *current,*prev;

    prev=&stage.bulletHead;

    for(current=stage.bulletHead.next;current!=NULL;prev=current,current=current->next)
    {
        current->x+=current->dx;
        current->y+=current->dy;
        
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

//Checks if the bullet has collided with any ships. Including boss ships
static int bulletHit(Entity *bullet)
{
    //we check each fighter to see if it collided. 
    Entity *current;

    for(current=stage.fighterHead.next;current!=NULL;current=current->next)
    {
        if(bullet->side!=current->side && bullet->health==1 //if a ship is hit by an active bullet of another side
            && collision(bullet->x,bullet->y,bullet->w,bullet->h,current->x,current->y,current->w,current->h))
        {
            bullet->health=0;
            current->health-=1;
            return 1;
        }

        if(activeBoss!=NULL && bullet->side!=activeBoss->side && bullet->health==1  //if an active boss is hit by an active bullet of another side
            && collision(bullet->x,bullet->y,bullet->w,bullet->h,activeBoss->x,activeBoss->y,activeBoss->w,activeBoss->h)) 
        {
            bullet->health=0;
            activeBoss->health-=1;
            return 1;
        }
    }

    return 0;

}

//Periodically make a new enemy and add it to the fighter list.
static void spawnEnemies()
{
    enemySpawnTime--;

    if(enemySpawnTime>0 || activeBoss!=NULL) //no new enemies spawn while a boss lives
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
    enemy->dy = -100 + (rand() % 200); //dy can now either be -1 or +1
    enemy->dy /= 100;
    enemy->health=1;

    
    enemy->texture=loadTexture("textures/enemy.png");
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

//Calls the series of functions that control the graphics of the game [drawShips,drawBullets]
static void draw(void)
{
    drawBackround(); //background.h
    drawStarfield(); //background.h

    drawPoints();
    drawBoss();
    drawDestroyed();
    drawShips();
    drawBullets();
    drawExplosions();
    drawDebris();
    drawHUD();
}

//draws every point in the list
static void drawPoints()
{
    Entity *current;
    for(current=stage.pointHead.next;current!=NULL;current=current->next)
    {
        if(current->health>=(3*FPS) || current->health % 12 < 6) // if it has more than 3 seconds of life alway draw it, if not make it blink
        {
            drawTexture(current->texture,current->x,current->y);
        }
    }
}

//If there is a boss present it will be drawn. 
static void drawBoss()
{
    if(activeBoss==NULL || activeBoss->x>SCREEN_WIDTH)
    {
        return;
    }

    drawModifiedTexture(activeBoss->texture,activeBoss->x,activeBoss->y,100,100);
}

//draws the destroy animation for each ship
static void drawDestroyed()
{
    SDL_Rect rect;
    Entity *current;
    for(current=stage.destroyedHead.next;current!=NULL;current=current->next)
    {
        rect.y=0;
        rect.h=current->h;
        rect.w=current->w; //there are 15 phases in the destroy texture
        rect.x=rect.w*(current->health/12); // since it "lives for 3 seconds" each phase lasts for 60*3/15 = 12 frames. 

        if(current==destructionSequenceBoss)
        {
            drawPartialModifiedTexture(current->texture,&rect,current->x-41,current->y-50,200,200);
        }
        else
        {
            drawPartialTexture(current->texture,&rect,current->x,current->y);
        }
    }
}

//Draws every ship on screen with updated coordinates
static void drawShips(void)
{
    Entity *current;
    for(current=stage.fighterHead.next;current!=NULL;current=current->next)
    {
        if(current==activeBoss)
        {
            drawModifiedTexture(current->texture,current->x,current->y,100,100);
        }
        else
        {
            drawTexture(current->texture,current->x,current->y);
        }
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

    SDL_Texture *explo=loadTexture("textures/explosion.png");
    SDL_SetRenderDrawBlendMode(app.renderer,SDL_BLENDMODE_ADD);
    //Enable additive blending, when adding textures one upon the other they blend colours
    SDL_SetTextureBlendMode(explo,SDL_BLENDMODE_ADD);

    for(e=stage.explosionHead.next;e!=NULL;e=e->next)
    {
        SDL_SetTextureColorMod(explo,e->r,e->g,e->b);
        SDL_SetTextureAlphaMod(explo,e->a);

        drawTexture(explo,e->x,e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);


}

//Displayes the text for score and highscore
static void drawHUD()
{
    if(warningBlink>FPS)
    {
        warningBlink=0;
    }
    //drawText supports variable arguments
    drawText(10,10,text_left,255,255,255,"SCORE: %03d",stage.score);
    if(player!=NULL)
    {
        drawText(10,39,text_left,255,player->health*20,player->health*20,"HP:%d",player->health);
    }
    else
    {
        drawText(10,39,text_left,255,0,0,"WARSHIP DESTROYED");
    }
    

    if(stage.score>0 && stage.score>=app.highScore) //passed the hightscore
    {
        drawText(1000,10,text_left,0,255,0,"HIGHSCORE: %03d",app.highScore);
    }
    else
    {
        drawText(1000,10,text_left,255,255,255,"HIGHSCORE: %03d",app.highScore);
    }

    if(activeBoss!=NULL && activeBoss->x>SCREEN_WIDTH && ++warningBlink>FPS/2)
    {
        drawText(400,40,text_left,255,0,0,"!!INCOMING CAPITAL SHIP!!");
    }
    else if(activeBoss!=NULL && activeBoss->x<SCREEN_WIDTH)
    {
        drawText(600,40,text_center,200,100,255,"BOSS HP:%d",activeBoss->health);
    }
}