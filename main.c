#include "sdl_playground.h"
#include "graphics.h"
#include "game_manager.h"
#include "audio_manager.h"

void clean_up(Player* player, UI* ui, Map* map, Audio* audio, Enemies* enemies, SDL_Renderer* renderer, SDL_Window* window);
void game(Player* player, Map* map, Audio* audio, Enemies* enemies, UI* ui, SDL_Renderer* renderer, SDL_Window* window, SDL_Rect* render_rect, SDL_Rect* tile_selector, Player_Prefs* player_prefs);

int main()
{
    // Startup
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    
    // Initialize SDL
    init_SDL(&renderer, &window);    
    SDL_Event event;
    bool quit = false;
    bool selected = false;

    // Instanciate objects
    Player_Prefs player_prefs = {.difficulty = 1, .number_of_ghosts = 4, .enemies_move_this_turn = false, .program_state = 0};
    Player player = {.health = 3, .score = 0, .player_sprite = NULL, .starting_x = 14, .starting_y = 23};
    Map map = {.full_points = 0};
    UI ui = {.displayed_score = -1, .selected_item = 0};
    Audio audio = {.bg_muted = false};
    Enemies enemies = {.number_of_enemies = player_prefs.number_of_ghosts};
    

    // Load graphic
    load_tile_sets(renderer, window, &player, &map, &enemies);    
    load_map_data(&map);
    load_ui_elements(renderer, window, &ui);
    load_highscore(&player_prefs);

    // Set starting values
    create_ghosts(&enemies);
    reset_player(&player);
    setup_nodes(&map);
    reset_nodes(&map);
    player_prefs.remaining_points = map.full_points;
    
    //Load music
    load_music(&audio);
    toggle_background_music(&audio);

    // Map, player, enemy sprite selector rect
    SDL_Rect tile_selector = {.x = 0, .y = 0, .w = 16, .h = 16};

    // Render rect
    SDL_Rect render_rect = {.x = 0, .y = 0, .w = 0, .h = 0};

    // Game loop
    while (!quit)
    {
        //Eval Event
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    // Move up
                    if (player_prefs.program_state == 1){
                        player.new_direction = 'U';
                    }
                    // Select higher menu item if there is
                    else if (player_prefs.program_state == 0 && ui.selected_item != 0)
                    {
                        ui.selected_item--;
                    }                    
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    // Move left
                    if (player_prefs.program_state == 1){
                        player.new_direction = 'L';
                    }                    
                    // Change settings
                    else if (player_prefs.program_state == 0)
                    {
                        switch (ui.selected_item)
                        {
                        case 1:
                            if (player_prefs.difficulty > 0){
                                player_prefs.difficulty--;
                            }
                            break;
                        case 2:
                            if (enemies.number_of_enemies > 1){
                                enemies.number_of_enemies--;
                                player_prefs.number_of_ghosts = enemies.number_of_enemies;
                            }
                            break;
                        case 3:
                            if (audio.bg_muted){
                                toggle_background_music(&audio);
                                audio.bg_muted = false;
                            }
                            break;
                        }                        
                    }                   
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    // Move down
                    if (player_prefs.program_state == 1){
                        player.new_direction = 'D';
                    }
                    // Select lower menu item if there is
                    else if (player_prefs.program_state == 0 && ui.selected_item != 3)
                    {
                        ui.selected_item++;
                    }                    
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    // Move right
                    if (player_prefs.program_state == 1){
                        player.new_direction = 'R';
                    }
                    // Change settings
                    else if (player_prefs.program_state == 0)
                    {
                        switch (ui.selected_item)
                        {
                        case 1:
                            if (player_prefs.difficulty < 1){
                                player_prefs.difficulty++;
                            }
                            break;
                        case 2:
                            if (enemies.number_of_enemies < 4){
                                enemies.number_of_enemies++;
                                player_prefs.number_of_ghosts = enemies.number_of_enemies;
                            }
                            break;
                        case 3:
                            if (!audio.bg_muted){
                                toggle_background_music(&audio);
                                audio.bg_muted = true;
                            }
                            break;   
                        }                     
                    }                     
                    break;
                case SDL_SCANCODE_RETURN:
                    if (player_prefs.program_state == 0 || player_prefs.program_state == 2){
                        selected = true;
                    }                    
                    break;
                }
                break;
            }
        }

        // Draw black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);                      

        // Main menu
        if (player_prefs.program_state == 0){
            render_main_menu(renderer, &ui, &render_rect, &player_prefs, &audio);

            // Start game
            if (selected && ui.selected_item == 0){                
                player_prefs.program_state = 1;
                new_game(&player);
                selected = false;
            }
        }

        // Game
        else if (player_prefs.program_state == 1){
            game(&player, &map, &audio, &enemies, &ui, renderer, window, &render_rect, &tile_selector, &player_prefs);
        }

        // Post mortal
        else if (player_prefs.program_state == 2){
            render_game_over(renderer, &ui, &render_rect, &player_prefs, &player);
            // Go back to main menu
            if (selected){                
                player_prefs.program_state = 0;
                selected = false;
            }
        }

              
    }    

    // Clean up
    clean_up(
        &player,
        &ui,
        &map,
        &audio,
        &enemies,
        renderer,
        window
    );
    

    return 0;
}

// Clean up
void clean_up(Player* player, UI* ui, Map* map, Audio* audio, Enemies* enemies, SDL_Renderer* renderer, SDL_Window* window){
    SDL_DestroyTexture(player->player_sprite);
    SDL_DestroyTexture(map->map_tile_map);
    SDL_DestroyTexture(ui->hearth_sprite);
    SDL_DestroyTexture(ui->score_tag);
    SDL_DestroyTexture(enemies->ghosts_sprite);
    SDL_DestroyTexture(ui->name_text);
    SDL_DestroyTexture(ui->main_menu_text);
    SDL_DestroyTexture(ui->game_over);
    SDL_DestroyTexture(ui->game_over_highscore);
    SDL_DestroyTexture(ui->game_over_score);
    TTF_CloseFont(ui->font);
    TTF_CloseFont(ui->small_font);
    TTF_CloseFont(ui->properties_font);
    sdl_playground_destroy(window, renderer);
    Mix_FreeMusic(audio->background_music);
    Mix_FreeChunk(audio->coin_sfx);
    Mix_FreeChunk(audio->death_sfx);
    Mix_FreeChunk(audio->level_finish_sfx);

    for (int i = 0; i < map->map_data_size; i++)
    {
        free(map->map_nodes[i].connected_nodes);
    }    

    free(map->map_nodes);
    free(enemies->ghosts);
}

// Game portion of progam
void game(Player* player, Map* map, Audio* audio, Enemies* enemies, UI* ui, SDL_Renderer* renderer, SDL_Window* window, SDL_Rect* render_rect, SDL_Rect* tile_selector, Player_Prefs* player_prefs){
    // Change movement direction if key was pressed and the field in direction is clear
        if (player->new_direction != '0'){
            // Move player
            move_player(player, map, player->new_direction, audio, player_prefs, enemies);

            // Move enemies if player started moving 
            if (player_prefs->difficulty == 1){
                move_enemies(map, enemies, player); 
            }
            else if(player_prefs->difficulty == 0 && player_prefs->enemies_move_this_turn){
                printf("1");
                move_enemies(map, enemies, player); 
                player_prefs->enemies_move_this_turn = false;
            }
            else{
                player_prefs->enemies_move_this_turn = true;
            }
                       
        }

        // Render stuff
        render_map(renderer, render_rect, tile_selector, map);
        render_player(renderer, render_rect, tile_selector, player);
        render_enemies(renderer, render_rect, tile_selector, enemies);
        render_ui(renderer, window, player, ui, render_rect);

        // Update the screen
        promote_render_to_screen(renderer);  

        // Level cleared?
        if (player_prefs->remaining_points == 0){
            level_cleared(audio, player_prefs, enemies, player, map);
        }

        // Check for collision
        if (check_collision(player, enemies)){
            player_died(player, enemies, map, audio, player_prefs);
            if (player->health < 1){
                // Change to game over
                player_prefs->program_state = 2;

                // Check highscore, if score higher -> update
                try_to_save_highscore(player_prefs, player);
            }  
            SDL_PumpEvents();
            SDL_FlushEvents(SDL_KEYDOWN, SDL_KEYUP);        
        }
}
