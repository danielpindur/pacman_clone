#include "app.h"
#include "graphics.h"
#include "game_controller.h"

int main(){

    Game game;
    game_ctor(&game);

    Player player;
    


    // Game loop
    SDL_Event event;
    bool quit = false;

    while (!quit)
    {
        //Eval Event
        while (SDL_PollEvent(&event))
        {
            switch (event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (game.gameState->state){
                        case Menu:
                            menuInputs(&game, &event);
                            break;

                        case Play:
                            gameInputs(&player, &event);
                            break;

                        case Over:
                            //TODO:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        clearScreen(game.app);            

        // Render
        switch (game.gameState->state){
            case Menu:
                renderMainMenu(&game);
                break;

            case Play:
                
                break;

            case Over:
                
                break;            
        }
/*
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

         */     
    }

    return EXIT_SUCCESS;
}