#ifndef SDL_PLAYGROUND_H_
#define SDL_PLAYGROUND_H_

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>



bool sdl_playground_init( SDL_Window ** win, SDL_Renderer ** renderer, const int window_width, const int window_height );
void sdl_playground_destroy( SDL_Window * win, SDL_Renderer * renderer );
void sdl_get_texture_from_image(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture ** texture);


#endif