#pragma once

#include "utils.h"
#include "app.h"

typedef struct node {
      int mapData; 
      V2 gridPos;  
      bool isWall;
      bool isVisited;
      struct node** connectedNodes;
      int numberOfConnectedNodes;
      struct node* parentNode;
      float globalGoal, localGoal;
} Node;

typedef struct {
    SDL_Texture* mapSpriteSheet;
    V2 mapLimit;
    int mapDataSize;
    int fullPoints;
    Node* mapNodes; 
} Map;