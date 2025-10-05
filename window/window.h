#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <time.h>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FONT_SIZE 18

#define FRAME_RATE 60
#define FRAME_DELAY 1000/FRAME_RATE

SDL_AppResult init_video();
SDL_AppResult init_window(SDL_Window **window);
SDL_AppResult init_renderer(SDL_Renderer **renderer,
                                SDL_Window *window);

#endif
