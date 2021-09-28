#pragma once

#include "app.h"
#include "ui.h"
#include "player.h"

enum State{Menu, Game, Over};

typedef struct {
      State state;
      int highscore;
      int remainingPoints;
      bool enemiesMoveThisTurn;
} GameState;

typedef struct {
      int numberOfEnemies;
      int difficulty;
} Settings;

typedef struct {
      App* app;
      UI* ui;
      GameState* gameState;
      Settings* settings;
} Game;

void menuInputs(Game* game, SDL_Event* event);
void gameInputs(Player* player, SDL_Event* event);