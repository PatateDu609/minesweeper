#pragma once

#include "structs.h"

#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

uint32_t *       sort(uint32_t *arr, int len);
char *           strjoin(const char *, const char *);
uint32_t *       find_sorted(uint32_t *arr, int len, uint32_t val);
t_score_numbers *itoscores(int digits, uint32_t nb);
void             set_pixel(SDL_Surface *surface, int x, int y, t_color color);
t_color          over_op(t_color bg, t_color fg);
SDL_Color        get_sdl_color(t_color c);
SDL_Surface *    create_rgb_surface(size_t width, size_t height);
t_color          get_color(int r, int g, int b, int a);
