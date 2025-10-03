#include "ball.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

void init_ball(Ball *ball, bool first_init){
    clock_t start_time_init, end_time;
    double time_taken;
    start_time_init = clock();
    ball->rect.y = (SCREEN_WIDTH - BALL_SIZE) / 1.5f;
    ball->rect.x = (SCREEN_HEIGHT - BALL_SIZE) / 2.0f;
    ball->rect.w = BALL_SIZE;
    ball->rect.h = BALL_SIZE;
    ball->dx = 200.0f;
    ball->dy = -200.0f;
    if (first_init) {
        ball->speed = 1.;
    }
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[GAME] Initialized ball in %f seconds.\n", time_taken) ;
}

void update_ball(Ball *ball, Paddle *paddle){
            ball->rect.x += (ball->dx * ball->speed) / FRAME_RATE;
            ball->rect.y += (ball->dy * ball->speed) / FRAME_RATE;

            // Ball collision with walls
            if (ball->rect.x < 2 || ball->rect.x > SCREEN_WIDTH - ball->rect.w - 2) {
                ball->dx = -ball->dx;
            }
            if (ball->rect.y < 2) {
                ball->dy = -ball->dy;
            }

            // Ball collision with paddle
            if (SDL_HasRectIntersectionFloat(&ball->rect, &paddle->rect)) {
                // Criar vetor de distância
                float vec_x = (ball->rect.x + ball->rect.w/2) - (paddle->rect.x + paddle->rect.w/2); 
                float vec_y = (ball->rect.y + ball->rect.h/2) - (paddle->rect.y + paddle->rect.h/2); 
                
                // Prevent ball from getting stuck in the paddle
                if (ball->dy > 0) {
                     ball->rect.y = paddle->rect.y - ball->rect.h;
                }
                ball->dy = -ball->dy;
            }
}

void render_ball(Ball *ball, SDL_Renderer *renderer, SDL_Color ball_color){
    SDL_SetRenderDrawColor(renderer, ball_color.r, ball_color.g, ball_color.b, ball_color.a);
    SDL_RenderFillRect(renderer, &ball->rect);
}
