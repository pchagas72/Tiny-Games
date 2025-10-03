#include "text.h"
#include <SDL3/SDL_render.h>

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

