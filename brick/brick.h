#ifndef BRICK_H
#define BRICK_H

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <time.h>
#include <stdio.h>
#include "../window/window.h"
#include "../ball/ball.h"
#include "../game/game.h"
#include "../menu/menu.h"

#define BRICK_WIDTH (SCREEN_WIDTH/settings->bricks_x)-(10)
#define BRICK_HEIGHT (SCREEN_HEIGHT/settings->bricks_y)/3

#define NUM_COLORS 7

typedef struct {
    SDL_FRect rect;
    bool active;
    SDL_Color color;
} Brick;

void init_bricks(Brick **bricks, Settings *settings, SDL_Color colors[], uint16_t *destroyed_bricks);
void update_bricks(Brick **bricks,
        Settings *settings,
        uint16_t *destroyed_bricks,
        Ball *ball,
        enum GAME_STATE *state);
void render_bricks(Brick **bricks, Settings *settings, SDL_Renderer *renderer);

#endif
