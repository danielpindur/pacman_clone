#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//#include "game_controller.h"

typedef struct {
      SDL_Window* window;
      int windowWidth;
      int windowHeight;
      SDL_Renderer* renderer;
      char* name;
} App;



// App constructor
void app_ctor(App* app, int windowWidth, int windowHeight,  char* name);

//void destroy_app(SDL_Window* win, SDL_Renderer* renderer);

//void sdl_get_texture_from_image(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture** texture);