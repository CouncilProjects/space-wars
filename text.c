#include"text.h"

SDL_Texture *fontTexture;
char textBuffer[MAX_LINE_LENGTH];

void initFont()
{
    fontTexture=loadTexture("textures/font.png");
}

void drawText(int x,int y,int r,int g,int b,char *format,...)
{
    int i,len,character;
    SDL_Rect rect;
    va_list args;

    //set the buffer with end of string symbols '\0'
    memset(&textBuffer,'\0',sizeof(textBuffer));

    //to start iteration in the va_list args
    va_start(args,format);

    //writes the formatted output (after "decoding" the format with the help of args) to the text buffer
    vsprintf(textBuffer,format,args);

    //to free the va_args
    va_end(args);

    len=strlen(textBuffer);

    rect.w=GLYPH_WIDTH;
    rect.h=GLYPH_HEIGHT;
    rect.y=0;

    //set the color of the text to be written
    SDL_SetTextureColorMod(fontTexture,r,g,b);

    /*now for every character in the string [1]
    find if its in font [2]
    if it is find its location [3]
    set the rext.x such that it takes the part of the texture that the character is in [4]
    set the x to the next GLYPGH_WIDTH so there is no ovelap [5]*/
    for(i=0;i<len;i++)//[1]
    {
        character=textBuffer[i];

        if(character>=' '&&character<='Z')//[2]
        {
            /*this is done to find where in the font the character is, 
            Example: if character is space (' ') then ' ' - ' ' = 0 so the x will be 0 so its the first "box" of the font*/
            rect.x=(character-' ')*GLYPH_WIDTH; //[3],[4]

            drawPartialTexture(fontTexture,&rect,x,y);

            //We set the x to the next GLYPH_WIDTH so the text has no overlap
            x+=GLYPH_WIDTH;//[5]
        }
    }
}