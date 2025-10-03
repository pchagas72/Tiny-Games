#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL_rect.h>
#include <time.h>
#include <stdio.h>
#include "../window/window.h"
#include "../paddle/paddle.h"
#define BALL_SIZE 15

typedef struct {
    SDL_FRect rect;
    float dx, dy;
    float speed;
} Ball;

void init_ball(Ball *ball, bool first_init);
void update_ball(Ball *ball, Paddle *paddle);
void render_ball(Ball *ball,
        SDL_Renderer *renderer,
        SDL_Color ball_color);

#endif
