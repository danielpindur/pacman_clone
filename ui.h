#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.h"
#include "game_controller.h"
#include "app.h"

typedef enum {Difficulty, NumberOfEnemies, MuteMusic} MenuItem;

typedef struct{
    SDL_Texture* hearthSprite;
    SDL_Texture* scoreTag;
    SDL_Texture* score;
    int displayedScore;
    TTF_Font* font;
    TTF_Font* smallFont;
    TTF_Font* propertiesFont;
    SDL_Color textColor;
    SDL_Color selectColor;
    int selectedItem; // TODO: Conv to enum
    SDL_Texture* nameText;
    SDL_Texture* gameOver;
    SDL_Texture* mainMenuText;
    SDL_Texture* gameOverScore;
    SDL_Texture* gameOverHighscore;
} UI;

void ui_ctor(UI* ui, App* app);