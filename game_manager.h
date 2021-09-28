#pragma once
#include "sdl_playground.h"
#include "audio_manager.h"
#include <math.h>

typedef struct {
    int number_of_ghosts;
    int difficulty;
    bool enemies_move_this_turn;
    int program_state;  // 0 - Main menu, 1 - Game, 2 - Post mortal
    int highscore;
    int remaining_points;
} Player_Prefs;

typedef struct node {
    int map_data; // What is is -> which sprite to render
    
    int x, y;  // Position on the grid
    bool is_wall;
    bool is_visited;
    struct node** connected_nodes;
    int number_of_connected_nodes;
    struct node* parent_node;
    float global_goal, local_goal;
} Node;

typedef struct {
    SDL_Texture* map_tile_map;
    int x_limit;
    int y_limit;
    int map_data_size;
    int full_points;
    Node* map_nodes; 
} Map;

typedef struct {
    int health;
    int score;
    int x_pos;
    int y_pos;
    char move_direction;
    char new_direction;
    bool open_mouth;
    bool moving;
    int starting_x, starting_y;
    SDL_Texture* player_sprite;
} Player;

typedef struct {
    int x_pos, y_pos;
} Ghost;

typedef struct {
    Ghost* ghosts;
    SDL_Texture* ghosts_sprite;
    int number_of_enemies;
} Enemies;

void load_map_data(Map* map);
void move_player(Player* player, Map* map, const char new_direction, Audio* audio, Player_Prefs* player_prefs, Enemies* enemies);
void create_ghosts(Enemies* enemies);
void setup_nodes(Map* map);
void reset_nodes(Map* map);
void move_enemies(Map* map, Enemies* enemies, Player* player);
void reset_player(Player* player);
void player_died(Player* player, Enemies* enemies, Map* map, Audio* audio, Player_Prefs* player_prefs);
bool check_collision(Player* player, Enemies* enemies);
void try_to_save_highscore(Player_Prefs* player_prefs, Player* player);
void load_highscore(Player_Prefs* player_prefs);
void new_game(Player* player);
void next_level(Map* map, Enemies* enemies, Player* player, Player_Prefs* player_prefs);
void level_cleared(Audio* audio, Player_Prefs* player_prefs, Enemies* enemies, Player* player, Map* map);