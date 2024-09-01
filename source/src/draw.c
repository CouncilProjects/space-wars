#include "draw.h"

extern App app;// i tell it there is indeed an app somewhere. 

//Draws the color of the scene.
void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

//Draw all new renderings that happend
void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

//Load the image from the specifide path, and make it to a texture Parameter[the path of the image] Return[Pointer to the image turned into SDL_texture]
SDL_Texture *loadTexture(char* fileName)
{
    SDL_Texture *texture;
    texture=getTexture(fileName);
    if(texture==NULL)
    {
        texture=IMG_LoadTexture(app.renderer,fileName);
        addToCache(fileName,texture);
    }
    
    return texture;
}

static SDL_Texture* getTexture(const char* filename)
{
    Texture *t;
    for(t=app.textureHead.next;t!=NULL;t=t->next)
    {
        if(strcmp(t->name,filename)==0)
        {
            return t->texture;
        }
    }

    return NULL;
}

static void addToCache(char* filename,SDL_Texture* texture)
{
    Texture *tex=malloc(sizeof(Texture));
    memset(tex,0,sizeof(Texture));

    tex->texture=texture;
    STRNCPY(tex->name,filename,MAX_NAME_LENGTH);

    app.textureTail->next=tex;
    app.textureTail=tex;

}


//Draw a texture at the coordinates x,y
void drawTexture(SDL_Texture *texture,int x,int y)
{
    SDL_Rect dest;
    dest.x=x;
    dest.y=y;
    //find the attributes of the texture (so that we know the w and h)
    SDL_QueryTexture(texture,NULL,NULL,&dest.w,&dest.h);

    //render a copy of the tecture. The null argument tell it to render it whole 
    SDL_RenderCopy(app.renderer,texture,NULL,&dest);
}

//Draw the src part of the given texture at coordinates x,y.
void drawPartialTexture(SDL_Texture *texture,SDL_Rect *src,int x,int y)
{
    SDL_Rect dest;
    //in dest put the size of the src
    dest.x=x;
    dest.y=y;
    dest.h=src->h;
    dest.w=src->w;
    
    //now draw it but only the part of the texture that src allows
    SDL_RenderCopy(app.renderer,texture,src,&dest);
}

//Draws an entire texture and displays it in a modified size increased by addexW X addedH at coordinates x,y.
void drawModifiedTexture(SDL_Texture *texture,int x,int y,int addedW,int addedH)
{
    SDL_Rect dest;
    dest.x=x;
    dest.y=y;
    SDL_QueryTexture(texture,NULL,NULL,&dest.w,&dest.h);
    dest.w+=addedW;
    dest.h+=addedH;

    SDL_RenderCopy(app.renderer,texture,NULL,&dest);
}

//Draws a part of a texture and displays it in a modified size increased by addexW X addedH at coordinates x,y.
void drawPartialModifiedTexture(SDL_Texture *texture,SDL_Rect *src,int x,int y,int addedW,int addedH)
{
    SDL_Rect dest;
    dest.x=x;
    dest.y=y;
    dest.w=src->w;
    dest.h=src->h;
    dest.w+=addedW;
    dest.h+=addedH;

    SDL_RenderCopy(app.renderer,texture,src,&dest);
}