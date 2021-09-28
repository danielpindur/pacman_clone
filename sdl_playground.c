#include "sdl_playground.h"


// Initialize all SDL
bool sdl_playground_init( SDL_Window ** win, SDL_Renderer ** renderer, const int window_width, const int window_height ) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    *win = SDL_CreateWindow("definitely not PACMAN", 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if ( !renderer ) {
        SDL_DestroyWindow( *win );
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }    
    if(TTF_Init() == -1) {        
        fprintf(stderr, "TTF_Init Error: %s", TTF_GetError());
        SDL_DestroyRenderer( *renderer );
        SDL_DestroyWindow( *win );
        SDL_Quit();
        assert(0);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        fprintf(stderr, "SDL_Mixer Error: %s", Mix_GetError());
        SDL_DestroyRenderer( *renderer );
        SDL_DestroyWindow( *win );
        TTF_Quit();
        SDL_Quit();
        assert(0);
    }
    

    return true;
}

// Terminates all SDL
void sdl_playground_destroy( SDL_Window * win, SDL_Renderer * renderer ) {
    if ( renderer )  {
        SDL_DestroyRenderer( renderer );
        renderer = NULL;
    }

    if ( win )  {
        SDL_DestroyWindow( win );
        win = NULL;
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

// Load the image into memory using SDL_image library function
bool sdl_get_surface_from_image(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Surface ** surface){    
    *surface = IMG_Load(path);    
    if (!surface)
    {
        fprintf(stderr, "SDL_CreateSurface Error: %s \n", SDL_GetError());
        sdl_playground_destroy(window, renderer);
        return 1;
    }
    return 0;
}

// Load the image data into the graphics hardware's memory
void sdl_get_texture_from_image(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture ** texture){
    SDL_Surface * surface;
    assert(!sdl_get_surface_from_image(path, renderer, window, &surface));

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!*texture)
    {
        fprintf(stderr, "SDL_CreateTexture Error: %s \n", SDL_GetError());
        sdl_playground_destroy(window, renderer);
        assert(0);
    }
}
