#ifndef MENU_H
#define MENU_H

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../game/game.h"

typedef struct {
    int selected_option;
    int num_menu_options;
    const char **options;
} Menu;

typedef struct {
    bool dark_mode;
    bool reset_game;
    uint8_t bricks_x;
    uint8_t bricks_y;
} Settings;

void init_menu(Menu *menu, const char **options, int num_options);
void handle_menu_events(Menu *menu, SDL_Event *event, enum GAME_STATE *state, enum GAME_STATE new_game_state, Settings *settings);
void render_menu(Menu *menu, const char* title, SDL_Renderer *renderer, TTF_Font *font, SDL_Color selected_color, SDL_Color normal_color);

#endif
