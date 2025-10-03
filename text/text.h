#ifndef TEXT_H
#define TEXT_H

#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../window/window.h"
#include <SDL3/SDL_render.h>
#include <time.h>
#include <stdio.h>

void render_text(const char* text, float x, float y, SDL_Color color, TTF_Font *font, SDL_Renderer *renderer);
SDL_AppResult init_font(TTF_Font **font);

#endif
