#include"sound.h"

Mix_Chunk *sound[SND_MAX];
Mix_Music *music;

void initSounds()
{
    memset(sound,0,sizeof(Mix_Chunk *)*SND_MAX);

    music=NULL;

    loadSounds();
    loadMusic("sound/backgroundMusic.ogg");
    playMusic(-1);
}

void loadSounds()
{
    
    sound[SND_PLAYER_FIRE]=Mix_LoadWAV("sound/fire.ogg");
    sound[SND_ALIEN_FIRE]=Mix_LoadWAV("sound/alienFire.ogg");
    sound[SND_PLAYER_DIE] = Mix_LoadWAV("sound/explosion.ogg");
    sound[SND_ALIEN_DIE]=Mix_LoadWAV("sound/explosionAlien.ogg");
}

//Load the music in memory to play later
void loadMusic(const char* fileName)
{
    
    //if there is a sound playing stop it.
    if(music!=NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music=NULL;
    }

    music=Mix_LoadMUS(fileName);
}

//Play the music, either play it forever or only once
void playMusic(int loop)
{
    Mix_PlayMusic(music,loop);
}

void playSound(int id,int channel)
{
    Mix_PlayChannel(channel,sound[id],0);
}