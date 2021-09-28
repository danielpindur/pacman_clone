#pragma once

#include "game_manager.h"

typedef struct{
    SDL_Texture* hearth_sprite;
    SDL_Texture* score_tag;
    SDL_Texture* score;
    int displayed_score;
    TTF_Font* font;
    TTF_Font* small_font;
    TTF_Font* properties_font;
    SDL_Color text_color;
    SDL_Color select_color;
    int selected_item;
    SDL_Texture* name_text;
    SDL_Texture* game_over;
    SDL_Texture* main_menu_text;
    SDL_Texture* game_over_score;
    SDL_Texture* game_over_highscore;
} UI;


void render_map(SDL_Renderer* renderer,SDL_Rect* render_rect, SDL_Rect* map_tile, Map* map);
void load_tile_sets(SDL_Renderer* renderer, SDL_Window* window, Player* player, Map* map, Enemies* enemies);
void set_map_limits(Map* map);
void render_player(SDL_Renderer* renderer, SDL_Rect* render_rect, SDL_Rect* player_spire_selector, Player* player);
void init_SDL(SDL_Renderer** renderer, SDL_Window** window);
void promote_render_to_screen(SDL_Renderer* renderer);
void load_ui_elements(SDL_Renderer* renderer, SDL_Window* window, UI* ui);
void render_ui(SDL_Renderer* renderer, SDL_Window* window, Player* player, UI* ui, SDL_Rect* render_rect);
void render_enemies(SDL_Renderer* renderer, SDL_Rect* render_rect, SDL_Rect* enemy_sprite_selector, Enemies* enemies);
void convert_to_2D(int index, int width, int* row, int* column);
void render_main_menu(SDL_Renderer* renderer, UI* ui, SDL_Rect* render_rect, Player_Prefs* player_prefs, Audio* audio);
void render_game_over(SDL_Renderer* renderer, UI* ui, SDL_Rect* render_rect, Player_Prefs* player_prefs, Player* player);