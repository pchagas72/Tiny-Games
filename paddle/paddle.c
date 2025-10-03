#include "paddle.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

void init_paddle(Paddle *paddle){
    clock_t start_time_init, end_time;
    double time_taken;
    start_time_init = clock();
    paddle->rect.x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
    paddle->rect.y = SCREEN_HEIGHT - PADDLE_HEIGHT - FONT_SIZE - 10.0f;
    paddle->rect.w = PADDLE_WIDTH;
    paddle->rect.h = PADDLE_HEIGHT;
    paddle->speedmult = 1.0f;
    paddle->speed = 500.0f;
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[GAME] Initialized paddle in %f seconds.\n", time_taken) ;
}

void update_paddle(Paddle *paddle, const bool *keyboard_state){
    if (keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_A]) {
        paddle->rect.x -= (paddle->speed * paddle->speedmult) / FRAME_RATE;
    }
    if (keyboard_state[SDL_SCANCODE_RIGHT] || keyboard_state[SDL_SCANCODE_D]) {
        paddle->rect.x += (paddle->speed * paddle->speedmult) / FRAME_RATE;
    }

    // Paddle bounds checking
    if (paddle->rect.x < 0) {
        paddle->rect.x = 0;
    }
    if (paddle->rect.x > SCREEN_WIDTH - paddle->rect.w) {
        paddle->rect.x = SCREEN_WIDTH - paddle->rect.w;
    }
}

void render_paddle(Paddle *paddle,
        SDL_Renderer *renderer,
        SDL_Color paddle_color){
            SDL_SetRenderDrawColor(renderer, paddle_color.r, paddle_color.g, paddle_color.b, paddle_color.a);
            SDL_RenderFillRect(renderer, &paddle->rect);
}
