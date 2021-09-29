#include "ui.h"

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

void ui_ctor(UI* ui, App* app){
      // Font and color setup
      ui->font = TTF_OpenFont(TEXT_FONT, 32);
      ui->smallFont = TTF_OpenFont(TEXT_FONT, 25);
      ui->propertiesFont = TTF_OpenFont(PROPERTIES_FONT, 20);
      ui->textColor.r = WHITE_R;
      ui->textColor.g = WHITE_G;
      ui->textColor.b = WHITE_B;
      ui->textColor.a = COLOR_A;
      ui->selectColor.r = RED_R;
      ui->selectColor.g = RED_G;
      ui->selectColor.b = RED_B;
      ui->selectColor.a = COLOR_A;

      // Load hearth sprite
      sdlGetTextureFromImage(HEARTH_SPRITE, app->renderer, app->window, ui->hearthSprite);
      
      // Load score tag text    
      SDL_Surface* textSurf = TTF_RenderText_Solid(ui->font, "SCORE", ui->textColor);
      ui->scoreTag = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);
      
      // Load name text    
      textSurf = TTF_RenderText_Solid(ui->font, "definitely not PACMAN", ui->textColor);
      ui->nameText = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);

      // Load game over text    
      textSurf = TTF_RenderText_Solid(ui->font, "GAME OVER", ui->textColor);
      ui->gameOver = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);

      textSurf = TTF_RenderText_Solid(ui->smallFont, "MAIN MENU", ui->selectColor);
      ui->mainMenuText = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);

      textSurf = TTF_RenderText_Solid(ui->smallFont, "SCORE", ui->textColor);
      ui->gameOverScore = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);

      textSurf = TTF_RenderText_Solid(ui->smallFont, "HIGHSCORE", ui->textColor);
      ui->gameOverHighscore = SDL_CreateTextureFromSurface(app->renderer, textSurf);
      SDL_FreeSurface(textSurf);      
}