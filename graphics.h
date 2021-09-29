#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game_controller.h"

void clearScreen(App* app);
void renderMainMenu(Game* game);
void sdlGetSurfaceFromImage(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Surface* surface);
void sdlGetTextureFromImage(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture);