#ifndef PADDLE_H
#define PADDLE_H

#include <SDL3/SDL_rect.h>
#include <time.h>
#include <stdio.h>
#include "../window/window.h"
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20

typedef struct {
    SDL_FRect rect;
    float speed;
    float speedmult;
} Paddle;

void init_paddle(Paddle *paddle);
void update_paddle(Paddle *paddle, const bool *keyboard_state);
void render_paddle(Paddle *paddle,
        SDL_Renderer *renderer,
        SDL_Color paddle_color);

#endif
