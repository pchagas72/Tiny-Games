#include "window.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_video.h>
#include <time.h>
#include <stdio.h>

SDL_AppResult init_video(){
    clock_t start_time_init, end_time;
    double time_taken;
    printf("[SDL] Initializing video...\n") ;
    start_time_init = clock();
    if (!SDL_Init(SDL_INIT_VIDEO)){
        printf("[SDL] Could not initialize video.\n") ;
        return SDL_APP_FAILURE;
    }
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[SDL] Initialized video in %f seconds.\n", time_taken) ;
    return SDL_APP_CONTINUE;
}

SDL_AppResult init_window(SDL_Window **window){
    clock_t start_time_init, end_time;
    double time_taken;
    printf("[SDL] Initializing window...\n") ;
    start_time_init = clock();
    *window = SDL_CreateWindow("Breakout",
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_MAXIMIZED);
    if (!window){
        printf("[SDL] Could not initialize window.\n") ;
        return SDL_APP_FAILURE;
    }
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[SDL] Initialized window in %f seconds.\n", time_taken) ;

    return SDL_APP_CONTINUE;
}
SDL_AppResult init_renderer(SDL_Renderer **renderer, SDL_Window *window){
    clock_t start_time_init, end_time;
    double time_taken;
    printf("[SDL] Initializing renderer...\n") ;
    start_time_init = clock();
    *renderer = SDL_CreateRenderer(window, NULL);
    if (!*renderer){
        printf("[SDL] Could not initialize renderer.\n") ;
        printf("[SDL-ERROR] %s\n", SDL_GetError()) ;
        return SDL_APP_FAILURE;
    }
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    SDL_SetRenderLogicalPresentation(*renderer, 600, 600, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    printf("[SDL] Initialized renderer in %f seconds.\n", time_taken) ;


    return SDL_APP_CONTINUE;
}
