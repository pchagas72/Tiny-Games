#include "menu/menu.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_surface.h>
#include <stdint.h>
#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "window/window.h"
#include "game/game.h"
#include "paddle/paddle.h"
#include "ball/ball.h"
#include "brick/brick.h"
#include "text/text.h"

// Declaring SDL variables
static SDL_Window *window = NULL; // Initializing SDL window pointer
static SDL_Renderer *renderer = NULL; // Initializing SDL renderer pointer
TTF_Font *font = NULL;

// Declaring color variables
SDL_Color colors[NUM_COLORS] = {
    {231, 130, 132, 255},       // Red
    {166, 218, 149, 255},       // Green
    {138, 173, 244, 255},       // Blue
    {238, 212, 159, 255},       // Yellow
    {139, 213, 202, 255},       // Teal
    {198, 160, 246, 255},       // mauve
    {245, 169, 127, 255}        // Peach
};
SDL_Color background_color_dark = {41,44,60,255};
SDL_Color background_color_light = {239,241,245,255};
SDL_Color paddle_darkmode = {198, 208, 245, 255};
SDL_Color paddle_lightmode = {76, 79, 105, 255};
SDL_Color ball_darkmode = {165, 173, 206, 255};
SDL_Color ball_lightmode = {108, 111, 133, 255};

// Menu variables
Menu main_menu;
const char *main_menu_options[] = {"Play", "Quit"};
Menu pause_menu;
const char* pause_menu_options[] = {"Resume", "Toggle Theme", "Menu"};

// Settings
Settings settings;

// Declaring GAME variables
enum GAME_STATE state = MENU;
Paddle paddle;
Ball ball;
Brick **bricks;
uint16_t destroyed_bricks;
uint32_t start_ticks;
uint32_t elapsed_milliseconds;
uint32_t pause_start_ticks; // To store when the pause began

/// This function runs once at the start of the program
/// Initialize stuff here
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv){
    // Initializes all the SDL subsystems
    clock_t start_time_init, end_time;
    double time_taken;
    start_time_init = clock();
    init_video();
    init_window(&window);
    init_renderer(&renderer, window);
    init_font(&font);
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init))/CLOCKS_PER_SEC;
    printf("[SDL] Initialized all subsystems in %f seconds.\n", time_taken) ;

    /// Initializing gmae objects
    printf("[GAME] Initializing game objects.\n") ;
    init_paddle(&paddle);
    init_ball(&ball, true);
    settings.dark_mode = false;
    settings.reset_game = false;
    settings.bricks_x = 10;
    settings.bricks_y = 5;

    bricks = (Brick **)malloc(settings.bricks_y * sizeof(Brick *));
    if (bricks == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for bricks rows.");
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < settings.bricks_y; i++){
        bricks[i] = (Brick *)malloc(settings.bricks_x * sizeof(Brick));
        if (bricks[i] == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for bricks columns.");
            // Free the already allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(bricks[j]);
            }
            free(bricks);
            return SDL_APP_FAILURE;
        }
    }

    init_bricks(
            bricks,
            &settings,
            colors,
            &destroyed_bricks
            );

    init_menu(&main_menu, main_menu_options, sizeof(main_menu_options) / sizeof(main_menu_options[0]));
    init_menu(&pause_menu, pause_menu_options, sizeof(pause_menu_options) / sizeof(pause_menu_options[0]));


    start_ticks = SDL_GetTicks();
    return SDL_APP_CONTINUE;
}

/// This function runs every time an event happens
/// An event could be a keypress
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){

    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }

    if (state == MENU){
        handle_menu_events(&main_menu, event, &state, PLAY, &settings);
    }

    if (event->type == SDL_EVENT_KEY_DOWN){
        if (event->key.key == SDLK_F){
            settings.dark_mode = !settings.dark_mode;
        }
        if (event->key.key == SDLK_E && state == PLAY){
            if (ball.speed < 10){
                ball.speed += 0.5;
                paddle.speed += 0.25;
            }
        }
        if (event->key.key == SDLK_Q && state == PLAY){
            if (ball.speed > 0.5){
                ball.speed -= 0.5;
            }
        }
        if (event->key.key == SDLK_ESCAPE){
            if (state == PAUSE) {
                uint32_t pause_duration = SDL_GetTicks() - pause_start_ticks;
                start_ticks += pause_duration;
                state = PLAY;
            }
            else if (state == PLAY) {
                // When pausing, record the time
                pause_start_ticks = SDL_GetTicks();
                state = PAUSE;
            }
        }
    }
    // Menu navigation only in PAUSE state
    if (state == PAUSE) {
        handle_menu_events(&pause_menu, event, &state, PLAY, &settings);
    }

    if (state == LOST || state == WIN){
        if (event->type == SDL_EVENT_KEY_DOWN){
            if (event->key.key == SDLK_Q){
                state = MENU;
            }
            if (event->key.key == SDLK_R){
                /// Initializing game objects
                clock_t start_time_reset = clock();
                double time_it_took;
                init_paddle(&paddle);
                init_ball(&ball, false);
                init_bricks(
                        bricks,
                        &settings,
                        colors,
                        &destroyed_bricks);

                start_ticks = SDL_GetTicks();
                clock_t end_time = clock();
                time_it_took = (double)(end_time - start_time_reset)/CLOCKS_PER_SEC;
                printf("[GAME] Rebuilt game in %f seconds\n", time_it_took);
                state = PLAY;
            }
        }
    }
    if (settings.reset_game) {
        /// Initializing game objects
        clock_t start_time_reset = clock();
        double time_it_took;
        init_paddle(&paddle);
        init_ball(&ball, false);
        init_bricks(
                bricks,
                &settings,
                colors,
                &destroyed_bricks);

        start_ticks = SDL_GetTicks();
        clock_t end_time = clock();
        time_it_took = (double)(end_time - start_time_reset)/CLOCKS_PER_SEC;
        settings.reset_game = false;
        printf("[GAME] Rebuilt game in %f seconds\n", time_it_took);
    }

    return SDL_APP_CONTINUE;
}

/// This function runs every frame
SDL_AppResult SDL_AppIterate(void *appstate){
    const Uint64 frame_start_time = SDL_GetTicks(); // Get start frame
    const bool *keyboard_state = SDL_GetKeyboardState(NULL); // Get kb state

    // Update game state
    switch (state) {

        case MENU:
            break;

        case PLAY:
            // Timer only updates when the game is in the PLAY state.
            elapsed_milliseconds = SDL_GetTicks() - start_ticks;
            // Paddle movement
            update_paddle(&paddle, keyboard_state);
            // Ball Movement
            update_ball(&ball, &paddle);
            // Brick and game state update
            update_bricks(
                    bricks,
                    &settings,
                    &destroyed_bricks,
                    &ball,
                    &state);
            break;

        case PAUSE:
            break;

        case LOST:
            break;

        case WIN:
            break;
    }
   // Rendering
    SDL_Color background_color = settings.dark_mode ? background_color_dark : background_color_light;
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Define colors for menu text
    SDL_Color selectedColor = settings.dark_mode ? (SDL_Color){238, 212, 159, 255} : (SDL_Color){138, 173, 244, 255}; // Yellow / Blue
    SDL_Color normalColor = settings.dark_mode ? paddle_darkmode : paddle_lightmode;

    // Render based on game state
    switch (state) {
        case MENU: {
            render_menu(&main_menu, "Breakout", renderer, font, selectedColor, normalColor);
            break;
        }

        case PLAY:
        case PAUSE: { // For both PLAY and PAUSE, we draw the game objects
            // Render paddle
            SDL_Color paddle_color = settings.dark_mode ? paddle_darkmode : paddle_lightmode;
            render_paddle(&paddle, renderer, paddle_color);

            // Render ball
            SDL_Color ball_color = settings.dark_mode ? ball_darkmode : ball_lightmode;
            render_ball(&ball,
                    renderer,
                    ball_color);

            // Render bricks
            render_bricks(bricks, &settings,
                     renderer);
            // Render HUD
            float elapsed_seconds = elapsed_milliseconds / 1000.0f;
            char timer_text[32], speed_text[32];
            snprintf(timer_text, sizeof(timer_text), "Time: %.2fs", elapsed_seconds);
            snprintf(speed_text, sizeof(speed_text), "Speed: %.2fx", ball.speed);
            render_text(timer_text, 5, SCREEN_HEIGHT - FONT_SIZE - 5, normalColor, font, renderer);
            render_text(speed_text, SCREEN_WIDTH - 5 - 12 * FONT_SIZE, SCREEN_HEIGHT - FONT_SIZE - 5, normalColor, font, renderer);

            // If paused, draw the overlay and menu
            if (state == PAUSE) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
                SDL_FRect overlayRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderFillRect(renderer, &overlayRect);
                render_menu(&pause_menu, "Paused", renderer, font, selectedColor, normalColor);
            }
            break;
        }

        case LOST: {
            render_text("You lost!", (SCREEN_WIDTH - 9 * FONT_SIZE) / 2.0f, (SCREEN_HEIGHT - FONT_SIZE) / 2.0f, normalColor, font, renderer);
            render_text("Press R to restart.", (SCREEN_WIDTH - 19 * FONT_SIZE) / 2.0f, (SCREEN_HEIGHT / 2.0f) + 30.0f, normalColor, font, renderer);
            render_text("Or Q to go to the main menu.", (SCREEN_WIDTH - 28 * FONT_SIZE) / 2.0f, (SCREEN_HEIGHT / 2.0f) + 60.0f, normalColor, font, renderer);
            float elapsed_seconds = elapsed_milliseconds / 1000.0f;
            char win_time_text[32];
            snprintf(win_time_text, sizeof(win_time_text), "Your time: %.2fs", elapsed_seconds);
            render_text(win_time_text, (SCREEN_WIDTH - sizeof(win_time_text)/sizeof(char) * FONT_SIZE) / 2.0f, SCREEN_HEIGHT-FONT_SIZE-10, normalColor, font, renderer);
            break;
        }

        case WIN: {
            render_text("You win!", (SCREEN_WIDTH - 8 * FONT_SIZE) / 2.0f, (SCREEN_HEIGHT - FONT_SIZE) / 2.0f, normalColor, font, renderer);
            render_text("Press R to restart or Q to quit.", (SCREEN_WIDTH - 32 * FONT_SIZE) / 2.0f, (SCREEN_HEIGHT / 2.0f) + 30.0f, normalColor, font, renderer);
            float elapsed_seconds = elapsed_milliseconds / 1000.0f;
            char win_time_text[32];
            snprintf(win_time_text, sizeof(win_time_text), "Your time: %.2fs", elapsed_seconds);
            render_text(win_time_text, (SCREEN_WIDTH - sizeof(win_time_text)/sizeof(char) * FONT_SIZE) / 2.0f, SCREEN_HEIGHT-FONT_SIZE-10, normalColor, font, renderer);
            break;
        }
    }

    SDL_RenderPresent(renderer);

    // Limit frame rate
    const Uint64 frame_time = SDL_GetTicks() - frame_start_time;
    if (frame_time < FRAME_DELAY){
        SDL_Delay(FRAME_DELAY - frame_time);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    for (int i = 0; i < settings.bricks_y; i++){
        free(bricks[i]);
    }
    free(bricks);
    SDL_Quit();
    printf("[SDL] Cleaned up all resources.\n");
}
