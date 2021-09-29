#include "graphics.h"

#define TOP_BAR_HEIGHT 50
#define FRAME_RATE 9

void clearScreen(App* app){
      // Draw black background
      SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255); 
      SDL_RenderClear(app->renderer);  
}

// Load the image into memory using SDL_image library function
void sdlGetSurfaceFromImage(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Surface* surface){    
    surface = IMG_Load(path);    
    if (!surface)
    {
            fprintf(stderr, "SDL_CreateSurface Error: %s \n", SDL_GetError());
            //sdl_playground_destroy(window, renderer);
            exit(1);
    }
}

// Load the image data into the graphics hardware's memory
void sdlGetTextureFromImage(char* path, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture){
    SDL_Surface* surface;
    sdlGetSurfaceFromImage(path, renderer, window, surface);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
            fprintf(stderr, "SDL_CreateTexture Error: %s \n", SDL_GetError());
            //sdl_playground_destroy(window, renderer);
            exit(1);
    }
}

/*
// Render main menu element, n = which element on menu, i = which select element
void render_main_menu_element(SDL_Renderer* renderer, SDL_Rect* render_rect, UI* ui, char* text, int n, int i, TTF_Font* font){     
    SDL_Surface* text_surf = NULL;
    if (ui->selected_item == i){
        text_surf = TTF_RenderText_Solid(font, text, ui->select_color);
    }
    else{
        text_surf = TTF_RenderText_Solid(font, text, ui->text_color);
    }

    SDL_Texture* text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_QueryTexture(text_tex, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = TOP_BAR_HEIGHT + 50 + 26 * n;
    SDL_RenderCopy(renderer, text_tex, NULL, render_rect);

    SDL_FreeSurface(text_surf);
    SDL_DestroyTexture(text_tex);
}*/

// Puts rendered stuff from memory to screen and manages framerate
void promoteRenderToScreen(SDL_Renderer* renderer){
    SDL_RenderPresent(renderer);  
    // Set frame rate
    SDL_Delay(1000 / FRAME_RATE);
}

// Renders the main menu
void renderMainMenu(Game* game){
      SDL_Rect renderRect;

      // Render name
      SDL_QueryTexture(game->ui->nameText, NULL, NULL, &renderRect.w, &renderRect.h);
      renderRect.x = (game->app->windowWidth - renderRect.w) / 2;
      renderRect.y = TOP_BAR_HEIGHT;
      SDL_RenderCopy(game->app->renderer, game->app->nameText, NULL, &renderRect);

      /*      
      // Render all menu elements
      char ghost_c[2];
      sprintf(ghost_c, "%d", player_prefs->number_of_ghosts);

      render_main_menu_element(renderer, render_rect, ui, "Play", 0, 0, ui->small_font);
      render_main_menu_element(renderer, render_rect, ui, "Difficulty", 2, 1, ui->small_font);

      if (player_prefs->difficulty == 0){
            render_main_menu_element(renderer, render_rect, ui, "Easy", 3, 10, ui->properties_font);
      }
      else {
            render_main_menu_element(renderer, render_rect, ui, "Hard", 3, 10, ui->properties_font);
      }

      render_main_menu_element(renderer, render_rect, ui, "Number of ghosts", 5, 2, ui->small_font);
      render_main_menu_element(renderer, render_rect, ui, ghost_c, 6, 10, ui->properties_font);
      render_main_menu_element(renderer, render_rect, ui, "Music", 8, 3, ui->small_font);

      if (audio->bg_muted == true){
            render_main_menu_element(renderer, render_rect, ui, "Muted", 9, 10, ui->properties_font);
      }
      else {
            render_main_menu_element(renderer, render_rect, ui, "Playing", 9, 10, ui->properties_font);
      }
      
      */
      promoteRenderToScreen(game->app->renderer);
}