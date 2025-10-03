#include "text.h"
#include <SDL3/SDL_error.h>

void render_text(const char* text,
        float x,
        float y,
        SDL_Color color,
        TTF_Font *font,
        SDL_Renderer *renderer) {
    if (!font) return;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, 0, color);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_FRect destRect = {x, y, (float)textSurface->w, (float)textSurface->h};
            SDL_RenderTexture(renderer, textTexture, NULL, &destRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_DestroySurface(textSurface);
    }
}

SDL_AppResult init_font(TTF_Font **font){
    clock_t start_time_init, end_time;
    double time_taken;
    start_time_init = clock();
    if (!TTF_Init()){
        printf("[SDL] Could not init SDL_ttf\n");
        return SDL_APP_FAILURE;
    }
    end_time = clock();
    time_taken = ((double)(end_time - start_time_init)) / CLOCKS_PER_SEC;
    printf("[SDL] Initialized SDL_ttf in %f seconds.\n", time_taken) ;
    
    *font = TTF_OpenFont("res/PublicPixel.ttf", FONT_SIZE);
    if (!font){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

 
    return SDL_APP_CONTINUE;
}
