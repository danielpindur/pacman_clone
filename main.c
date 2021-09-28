#include "app.h"

int main(){
    
    //Init
    App app;
    char name[] = "definitely not PACMAN";
    app_ctor(&app, 448, 496, name);
    
    Game game;
    Player player;
    

    SDL_Event event;
    bool quit = false;


    // Game loop
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
                    switch (game.gameState){
                        case Menu:
                            menuInputs(&game, &event);
                            break;

                        case Game:
                            gameInputs(&player, &event);
                            break;

                        case Over:
                            //TODO:
                            break;
                    }
                    break;
            }
        }

        // Draw black background
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255); 
        SDL_RenderClear(app.renderer);                      

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