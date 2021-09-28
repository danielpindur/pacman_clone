#include "app.h"

// Initializes SDL
void initSDL(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        assert(0);
    }
}

// Initializes Window
void initWindow(App* app){
    app->window = SDL_CreateWindow(app->name, 100, 100, app->windowWidth, app->windowHeight, SDL_WINDOW_SHOWN);
    if (!app->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        assert(0);
    }
}

// Initializes Renderer
void initRenderer(App* app){
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer) {
        SDL_DestroyWindow(app->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        assert(0);
    }
}

/*
// Initializes TTF
void initTTF(App* app){
    if(TTF_Init() == -1) {        
        fprintf(stderr, "TTF_Init Error: %s", TTF_GetError());
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        assert(0);
    }
}

// Initializes Audio
void initAudio(App* app){
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_Mixer Error: %s", Mix_GetError());
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        TTF_Quit();
        SDL_Quit();
        assert(0);
    }
}
*/

// App constructor
void app_ctor(App* app, int windowWidth, int windowHeight, char* name){
    app->windowWidth = windowWidth;
    app->windowHeight = windowHeight;
    app->name = name;

    initSDL();
    initWindow(app);
    initRenderer(app);
}

// App destructor
/*
void app_dtor(App* app){
    
}
*/

/*
// Terminates all SDL
void destroy_app(SDL_Window* win, SDL_Renderer* renderer){
    if (renderer){
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (win){
        SDL_DestroyWindow(win);
        win = NULL;
    }
    
    Mix_CloseAudio();
    TTF_Quit();
    
    SDL_Quit();
}
*/