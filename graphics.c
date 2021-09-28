#include "sdl_playground.h"
#include "game_manager.h"
#include "graphics.h"


#define GAME_WIDTH 448
#define GAME_HEIGTH 496
#define TOP_BAR_HEIGHT 50
#define WINDOW_WIDTH 448

#define UI_TOP_OFFSET 9
#define UI_SIDE_OFFSET 5

#define TILE_SIZE 16
#define FRAME_RATE 9

#define PLAYER_SPRITE "../Sprites/pacman.png"
#define MAP_FILEPATH "../Sprites/map_tileset.png"
#define HEARTH_SPRITE "../Sprites/hearth.png"
#define GHOSTS_SPRITE "../Sprites/ghosts.png"

#define WHITE_R 255
#define WHITE_G 255
#define WHITE_B 255
#define RED_R 255
#define RED_G 0
#define RED_B 0
#define COLOR_A 255
#define TEXT_FONT "../Fonts/collegeb.ttf"
#define PROPERTIES_FONT "../Fonts/Roboto-Regular.ttf"

// Initialize SDL
void init_SDL(SDL_Renderer** renderer, SDL_Window** window){
    bool init = sdl_playground_init(window, renderer, WINDOW_WIDTH, GAME_HEIGTH + TOP_BAR_HEIGHT);
    if (!init){
        sdl_playground_destroy(*window, *renderer);
        assert(0);
    }
}

// Set map limits
void set_map_limits(Map* map){
    map->x_limit = (GAME_WIDTH / TILE_SIZE);
    map->y_limit = (GAME_HEIGTH / TILE_SIZE);
}

// Load graphics
void load_tile_sets(SDL_Renderer* renderer, SDL_Window* window, Player* player, Map* map, Enemies* enemies){    
    sdl_get_texture_from_image(PLAYER_SPRITE, renderer, window, &player->player_sprite);
    sdl_get_texture_from_image(MAP_FILEPATH, renderer, window, &map->map_tile_map);
    sdl_get_texture_from_image(GHOSTS_SPRITE, renderer, window, &enemies->ghosts_sprite);
}

// Converts 1D coords to 2D
void convert_to_2D(int index, int width, int* row, int* column) {
    *row = index / width;
    *column = index % width;
}


// Renders the map based on given map_data
void render_map(SDL_Renderer* renderer,SDL_Rect* render_rect, SDL_Rect* map_tile, Map* map){
        // Set render rect to render tiles
        render_rect->w = map_tile->w;
        render_rect->h = map_tile->h;
        map_tile->y = 0;

        for (int index = 0; index < map->map_data_size; index++) 
        {   
            // Get the right tile            
            int tile_var = map->map_nodes[index].map_data;
            if (tile_var == 0){
                continue;
            }
            else{
                map_tile->x = map_tile->w * tile_var;
            }

            // Get the rigth position on screen
            convert_to_2D(index, map->x_limit, &render_rect->y, &render_rect->x);
            render_rect->x *= TILE_SIZE;
            render_rect->y = render_rect->y * TILE_SIZE + TOP_BAR_HEIGHT;

            SDL_RenderCopy(renderer, map->map_tile_map, map_tile, render_rect);
        }
}

// Renders player
void render_player(SDL_Renderer* renderer, SDL_Rect* render_rect, SDL_Rect* player_sprite_selector, Player* player){
    // Set render rect to render player
    render_rect->w = player_sprite_selector->w;
    render_rect->h = player_sprite_selector->h; 

    // Which player sprite to place?
    player_sprite_selector->y = 0;
    
    // Open mouth
    if (player->open_mouth && player->moving){        
        switch (player->move_direction)
        {
        case 'U':
            player_sprite_selector->x = 1 * TILE_SIZE;
            break;
        case 'D':
            player_sprite_selector->x = 3 * TILE_SIZE;
            break;
        case 'L':
            player_sprite_selector->x = 4 * TILE_SIZE;
            break;
        case 'R':
            player_sprite_selector->x = 2 * TILE_SIZE;
            break;  
        } 
        player->open_mouth = false;
    }
    // Close mouth
    else
    {
        player_sprite_selector->x = 0;
        player->open_mouth = true;
    }    

    // Where to place player?
    render_rect->x = player->x_pos * TILE_SIZE;
    render_rect->y = player->y_pos * TILE_SIZE + TOP_BAR_HEIGHT;

    SDL_RenderCopy(renderer, player->player_sprite, player_sprite_selector, render_rect);
}

// Puts rendered stuff from memory to screen and manages framerate
void promote_render_to_screen(SDL_Renderer* renderer){
    SDL_RenderPresent(renderer);  
    // Set frame rate
    SDL_Delay(1000 / FRAME_RATE);
}

// Loads all UI elements texture          
void load_ui_elements(SDL_Renderer* renderer, SDL_Window* window, UI* ui){
    // Font and color setup
    ui->font = TTF_OpenFont(TEXT_FONT, 32);
    ui->small_font = TTF_OpenFont(TEXT_FONT, 25);
    ui->properties_font = TTF_OpenFont(PROPERTIES_FONT, 20);
    ui->text_color.r = WHITE_R;
    ui->text_color.g = WHITE_G;
    ui->text_color.b = WHITE_B;
    ui->text_color.a = COLOR_A;
    ui->select_color.r = RED_R;
    ui->select_color.g = RED_G;
    ui->select_color.b = RED_B;
    ui->select_color.a = COLOR_A;

    // Load hearth sprite
    sdl_get_texture_from_image(HEARTH_SPRITE, renderer, window, &ui->hearth_sprite);
    
    // Load score tag text    
    SDL_Surface* text_surf = TTF_RenderText_Solid(ui->font, "SCORE", ui->text_color);
    ui->score_tag = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    
    // Load name text    
    text_surf = TTF_RenderText_Solid(ui->font, "definitely not PACMAN", ui->text_color);
    ui->name_text = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);

    // Load game over text    
    text_surf = TTF_RenderText_Solid(ui->font, "GAME OVER", ui->text_color);
    ui->game_over = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);

    text_surf = TTF_RenderText_Solid(ui->small_font, "MAIN MENU", ui->select_color);
    ui->main_menu_text = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);

    text_surf = TTF_RenderText_Solid(ui->small_font, "SCORE", ui->text_color);
    ui->game_over_score = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);

    text_surf = TTF_RenderText_Solid(ui->small_font, "HIGHSCORE", ui->text_color);
    ui->game_over_highscore = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
}

// Renders whole UI
void render_ui(SDL_Renderer* renderer, SDL_Window* window, Player* player, UI* ui, SDL_Rect* render_rect){
    // Render hearths
    SDL_QueryTexture(ui->hearth_sprite, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = UI_SIDE_OFFSET;
    render_rect->y = UI_TOP_OFFSET;

    for (int i = 0; i < player->health; i++)
    {
        SDL_RenderCopy(renderer, ui->hearth_sprite, NULL, render_rect);
        render_rect->x += render_rect->w;
    }

    
    // Render score tag
    SDL_QueryTexture(ui->score_tag, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->y = UI_TOP_OFFSET;
    render_rect->x = WINDOW_WIDTH / 2 + 50;
    SDL_RenderCopy(renderer, ui->score_tag, NULL, render_rect);
    
    // Render score
    //Did score change since last render?
    if (player->score != ui->displayed_score){
        ui->displayed_score = player->score;
        char score_string[5];
        sprintf(score_string, "%d", ui->displayed_score);
        SDL_Surface* text_surf = TTF_RenderText_Solid(ui->font, score_string, ui->text_color);
        ui->score = SDL_CreateTextureFromSurface(renderer, text_surf);
        SDL_FreeSurface(text_surf);
    }

    SDL_QueryTexture(ui->score, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->y = UI_TOP_OFFSET;
    render_rect->x = WINDOW_WIDTH - render_rect->w - UI_SIDE_OFFSET;
    SDL_RenderCopy(renderer, ui->score, NULL, render_rect);
}

// Renders single ghost
void render_ghost(SDL_Renderer* renderer, SDL_Rect* render_rect, SDL_Rect* enemy_sprite_selector, Ghost* ghost, SDL_Texture* sprite_sheet){
    render_rect->x = ghost->x_pos * TILE_SIZE;
    render_rect->y = ghost->y_pos * TILE_SIZE + TOP_BAR_HEIGHT;

    SDL_RenderCopy(renderer, sprite_sheet, enemy_sprite_selector, render_rect);
}

// Render all enemies
void render_enemies(SDL_Renderer* renderer, SDL_Rect* render_rect, SDL_Rect* enemy_sprite_selector, Enemies* enemies){
    // Set up rects
    enemy_sprite_selector->w = TILE_SIZE;
    enemy_sprite_selector->h = TILE_SIZE;
    render_rect->w = enemy_sprite_selector->w;
    render_rect->h = enemy_sprite_selector->h;

    enemy_sprite_selector->x = 0;

    for (int i = 0; i < enemies->number_of_enemies; i++)
    {
        enemy_sprite_selector->y = i * TILE_SIZE;
        render_ghost(renderer, render_rect, enemy_sprite_selector, &enemies->ghosts[i], enemies->ghosts_sprite);
    }
    
}

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
}

// Renders the main menu
void render_main_menu(SDL_Renderer* renderer, UI* ui, SDL_Rect* render_rect, Player_Prefs* player_prefs, Audio* audio){
    // Render name
    SDL_QueryTexture(ui->name_text, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = TOP_BAR_HEIGHT;
    SDL_RenderCopy(renderer, ui->name_text, NULL, render_rect);
    
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
    
    promote_render_to_screen(renderer);
}

// Renders game over screen
void render_game_over(SDL_Renderer* renderer, UI* ui, SDL_Rect* render_rect, Player_Prefs* player_prefs, Player* player){
    // Render game over
    SDL_QueryTexture(ui->game_over, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = TOP_BAR_HEIGHT;
    SDL_RenderCopy(renderer, ui->game_over, NULL, render_rect);

    // Render highscore
    SDL_QueryTexture(ui->game_over_highscore, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = 2 * TOP_BAR_HEIGHT;
    SDL_RenderCopy(renderer, ui->game_over_highscore, NULL, render_rect);

    char hs_s[5];
    sprintf(hs_s, "%d", player_prefs->highscore);
    SDL_Surface* text_surf = TTF_RenderText_Solid(ui->properties_font, hs_s, ui->text_color);
    SDL_Texture* text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_QueryTexture(text_tex, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = 2 * TOP_BAR_HEIGHT + 30;
    SDL_RenderCopy(renderer, text_tex, NULL, render_rect);
    SDL_FreeSurface(text_surf);
    SDL_DestroyTexture(text_tex);

    // Render score
    SDL_QueryTexture(ui->game_over_score, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = 4 * TOP_BAR_HEIGHT;
    SDL_RenderCopy(renderer, ui->game_over_score, NULL, render_rect);

    char s_s[5];
    sprintf(s_s, "%d", player->score);
    text_surf = TTF_RenderText_Solid(ui->properties_font, s_s, ui->text_color);
    text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_QueryTexture(text_tex, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = 4 * TOP_BAR_HEIGHT + 30;
    SDL_RenderCopy(renderer, text_tex, NULL, render_rect);

    // Render main menu button
    SDL_QueryTexture(ui->main_menu_text, NULL, NULL, &render_rect->w, &render_rect->h);
    render_rect->x = (WINDOW_WIDTH - render_rect->w) / 2;
    render_rect->y = GAME_HEIGTH;
    SDL_RenderCopy(renderer, ui->main_menu_text, NULL, render_rect);

    SDL_FreeSurface(text_surf);
    SDL_DestroyTexture(text_tex);

    promote_render_to_screen(renderer);
}