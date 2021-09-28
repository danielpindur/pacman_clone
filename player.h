#pragma once

#include "app.h"
#include "utils.h"

enum Direction{Right, Up, Left, Down};

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