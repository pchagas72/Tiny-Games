#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../ball/ball.h"
#include "../brick/brick.h"
#include "../game/game.h"
#include "../menu/menu.h"
#include "../paddle/paddle.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

// GameState struct to hold all game-related data
typedef struct {
    enum GAME_STATE state;
    Paddle paddle;
    Ball ball;
    Brick **bricks;
    uint16_t destroyed_bricks;
    uint32_t start_ticks;
    uint32_t elapsed_milliseconds;
    uint32_t pause_start_ticks;

    // SDL variables
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    // Settings
    Settings settings;

    // Menus
    Menu main_menu;
    Menu pause_menu;

    // Keyboard state
    const bool *keyboard_state;
} GameState;

#endif
