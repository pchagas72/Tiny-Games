#include "brick.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

void init_bricks(Brick **bricks, Settings *settings, SDL_Color colors[], uint16_t *destroyed_bricks){
    clock_t start_time_init, end_time;
    double time_taken;
    start_time_init = clock();
    for (int y = 0; y < settings->bricks_y; y++) {
        for (int x = 0; x < settings->bricks_x; x++) {
            bricks[y][x] = (Brick){
                .rect = { (float)x * (BRICK_WIDTH + 10) + 5, (float)y * (BRICK_HEIGHT + 10) + 5, BRICK_WIDTH, BRICK_HEIGHT },
                .active = true,
                .color = colors[rand() % NUM_COLORS]
            };
        }
    }
    *destroyed_bricks = 0;
 
   end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[GAME] Initialized bricks in %f seconds.\n", time_taken) ;
}

void updated_bricks(Brick **bricks, Settings *settings, uint16_t *destroyed_bricks, Ball *ball, enum GAME_STATE *state){
            // Ball collision with bricks
            for (int y = 0; y < settings->bricks_y; y++) {
                for (int x = 0; x < settings->bricks_x; x++) {
                    if (bricks[y][x].active && SDL_HasRectIntersectionFloat(&ball->rect, &bricks[y][x].rect)) {
                        bricks[y][x].active = false;
                        ball->dy = -ball->dy;
                        (*destroyed_bricks)++;
                    }
                }
            }
            if (*destroyed_bricks == settings->bricks_x * settings->bricks_y) {
                *state = WIN; 
            }

            // Check for loss condition
            if (ball->rect.y > SCREEN_HEIGHT) {
                *state = LOST;
            }
}

void render_bricks(Brick **bricks, Settings *settings, SDL_Renderer *renderer){
    for (int y = 0; y < settings->bricks_y; y++) {
        for (int x = 0; x < settings->bricks_x; x++) {
            if (bricks[y][x].active) {
                SDL_SetRenderDrawColor(renderer, bricks[y][x].color.r, bricks[y][x].color.g, bricks[y][x].color.b, bricks[y][x].color.a);
                SDL_RenderFillRect(renderer, &bricks[y][x].rect);
            }
        }
    }
}
