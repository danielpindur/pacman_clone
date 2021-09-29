#pragma once

#include "app.h"
#include "utils.h"

typedef enum {Right, Up, Left, Down} Direction;

typedef struct {
    int health;
    int score;
    V2 position;
    Direction moveDirection;
    Direction newDirection;
    bool openMouth;
    bool moving;
    V2 startingPosition;
    SDL_Texture* playerSprite;
} Player;