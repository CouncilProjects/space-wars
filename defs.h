#ifndef DEF_H
#define DEF_H
//File that contains all definitions
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define MAX_KEYBOARD_KEYS 350
#define PLAYER_SIDE 0
#define ALIEN_SIDE 1
#define FPS 60
#define ALIEN_BULLET_SPEED    8
#define MAX_STARS 500

#define MAX_SND_CHANNELS 8

enum
{
    ch_any=-1,
    ch_player,
    ch_alien_fire
};

enum
{
    SND_PLAYER_FIRE,
    SND_ALIEN_FIRE,
    SND_PLAYER_DIE,
    SND_ALIEN_DIE,
    SND_MAX
};

#define MAX_LINE_LENGTH 1024


#define GLYPH_WIDTH 18
#define GLYPH_HEIGHT 28
#endif