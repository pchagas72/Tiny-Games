#include "menu.h"
#include "../window/window.h"
#include "../text/text.h"
#include <string.h>

void init_menu(Menu *menu, const char **options, int num_options) {
    menu->selected_option = 0;
    menu->options = options;
    menu->num_menu_options = num_options;
}

void handle_menu_events(Menu *menu,
        SDL_Event *event,
        enum GAME_STATE *state,
        enum GAME_STATE new_game_state,
        Settings *settings) {

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_UP) {
            menu->selected_option = (menu->selected_option - 1 + menu->num_menu_options) % menu->num_menu_options;
        } else if (event->key.key == SDLK_DOWN) {
            menu->selected_option = (menu->selected_option + 1) % menu->num_menu_options;
        } else if (event->key.key == SDLK_RETURN) {
            /// PAUSE MENU
            if (strcmp(menu->options[menu->selected_option], "Resume") == 0){
                *state = PLAY;
            }
            if (strcmp(menu->options[menu->selected_option], "Toggle Theme") == 0){
                settings->dark_mode = !settings->dark_mode;
            }
            if (strcmp(menu->options[menu->selected_option], "Menu") == 0){
                *state = MENU;
            }
            /// MAIN MENU
            if (strcmp(menu->options[menu->selected_option], "Play") == 0){
                *state = PLAY;
                settings->reset_game = true;
            }
            if (strcmp(menu->options[menu->selected_option], "Quit") == 0){
                SDL_Event quit_event;
                quit_event.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quit_event);
            }
        }
    }
}


void render_menu(Menu *menu, const char* title, SDL_Renderer *renderer, TTF_Font *font, SDL_Color selected_color, SDL_Color normal_color) {
    render_text(title, (SCREEN_WIDTH - strlen(title) * (FONT_SIZE + 10)) / 2.0f, 150.0f, selected_color, font, renderer);

    float menu_y_start = 250.0f;
    for (int i = 0; i < menu->num_menu_options; i++) {
        SDL_Color color = (i == menu->selected_option) ? selected_color : normal_color;
        const char* text = menu->options[i];
        render_text(text, (SCREEN_WIDTH - (strlen(text) * FONT_SIZE)) / 2.0f, menu_y_start + i * 40.0f, color, font, renderer);
    }
}
