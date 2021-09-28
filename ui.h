#pragma once

#include "app.h"

enum MenuItem{Difficulty, NumberOfEnemies, MuteMusic};

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