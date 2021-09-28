#include "game_controller.h"

void menuInputs(Game* game, SDL_Event* event){
      switch (event->key.keysym.scancode){
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                  // Select higher menu item if there is
                  if (game->ui->selectedItem != 0)
                  {
                        game->ui->selectedItem--;
                  }                    
                  break;

            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                  // Change settings
                  switch (game->ui->selected_item){
                        case Difficulty:
                              if (game->settings->difficulty > 0){
                                    game->settings->difficulty--;
                              }
                              break;

                        case NumberOfEnemies:
                              if (game->settings->numberOfEnemies > 1){
                                    game->settings->numberOfEnemies;
                              }
                              break;
                        
                        case MuteMusic:
                              // TODO
                              /*
                              if (audio.bg_muted){
                                    toggle_background_music(&audio);
                                    audio.bg_muted = false;
                              }
                              */
                              break;
                  }                        
                  break;

            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                  // Select lower menu item if there is
                  if (game->ui->selected_item != 3)
                  {
                        game->ui->selectedItem++;
                  }                    
                  break;

            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                  // Change settings
                  switch (game->ui->selectedItem){
                        case Difficulty:
                              if (game->settings->difficulty < 1){
                                    player_prefs.difficulty++;
                              }
                              break;
                        
                        case NumberOfEnemies:
                              if (game->settings->numberOfEnemies < 4){
                                    game->settings->numberOfEnemies++;
                              }
                              break;
                        case MuteMusic:
                              //TODO
                              /*
                              if (!audio.bg_muted){
                                    toggle_background_music(&audio);
                                    audio.bg_muted = true;
                              }
                              */
                              break;   
                  }                   
                  break;
            case SDL_SCANCODE_RETURN:
                  //selected = true; ????????                   
                  break;
      }               
}

void gameInputs(Player* player, SDL_Event* event){
      switch (event->key.keysym.scancode){
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                  // Move up
                  player->newDirection = Up;                 
                  break;

            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                  player->newDirection = Left;
                  break;                  
            
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                  // Move down
                  player->newDirection = Down;                 
                  break;

            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                  // Move right
                  player->newDirection = Right;           
                  break;
      }      
}