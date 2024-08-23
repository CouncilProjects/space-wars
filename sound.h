#ifndef SOUND_H
#define SOUND_H

#include"SDL_mixer.h"
#include"structs.h"

//nullifies pointer music, and sets array mem to 0 , calls functions [loadMusic,playMusic,loadSound] 
void initSounds();

//loads the sounds to the array
void loadSounds();

//Play the selected sound at the corresponding channel (both enumerated in def.h)
void playSound(int id,int channel);

//Load the music in memory to play later
void loadMusic(const char* fileName);

//Play the music, either playe it forever or only once
void playMusic(int loop);

#endif