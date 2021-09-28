#pragma once
#include "sdl_playground.h"

typedef struct{
    Mix_Music* background_music;
    Mix_Chunk* coin_sfx;
    Mix_Chunk* death_sfx;
    Mix_Chunk* level_finish_sfx;
    bool bg_muted;
} Audio;

void load_music(Audio* audio);
void toggle_background_music(Audio* audio);
void play_coin_sfx(Audio* audio);
void play_death_sfx(Audio* audio);
void play_level_finish_sfx(Audio* audio);