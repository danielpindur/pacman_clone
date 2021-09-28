#include "audio_manager.h"

#define BGM_FILE_PATH "../Sound/pacman_theme.wav"
#define COIN_SFX_FILE_PATH "../Sound/pickup.wav"
#define DEATH_SFX_FILE_PATH "../Sound/player_death_sfx.wav"
#define LEVEL_FINISH_SFX_FILE_PATH "../Sound/level_finish_sfx.wav"

// Loads all sound files
void load_music(Audio* audio){
    audio->background_music = Mix_LoadMUS(BGM_FILE_PATH);
    audio->coin_sfx = Mix_LoadWAV(COIN_SFX_FILE_PATH);
    audio->death_sfx = Mix_LoadWAV(DEATH_SFX_FILE_PATH);
    audio->level_finish_sfx = Mix_LoadWAV(LEVEL_FINISH_SFX_FILE_PATH);
}

// Toggles background music
void toggle_background_music(Audio* audio){
    if(!Mix_PlayingMusic()){
        Mix_PlayMusic(audio->background_music, -1);
    }
    else if (Mix_PausedMusic()){
        Mix_ResumeMusic();
    }
    else{
        Mix_HaltMusic();
    }
}

// Plays coin sfx
void play_coin_sfx(Audio* audio){
    Mix_PlayChannel(-1, audio->coin_sfx, 0);
}

//Plays death sfx
void play_death_sfx(Audio* audio){
    Mix_PlayChannel(-1, audio->death_sfx, 0);
}

//Play level finish sfx
void play_level_finish_sfx(Audio* audio){
    Mix_PlayChannel(-1, audio->level_finish_sfx, 0);
}