#pragma once

#include "ui/ui.decl.h"
#include "color.h"

#include <SDL_render.h>

#include <stdint.h>


struct stop
{
	float   offset;
	t_color color;
};


struct linear_gradient
{
	size_t nb_stops;
	t_stop stops[];
};


extern const t_linear_gradient grad_ui_header;

SDL_Texture *apply_grad(SDL_Renderer *renderer, SDL_Rect rect, const struct linear_gradient *grad);
t_color      apply_grad_pixel(SDL_Rect rect, SDL_Point pt, const struct linear_gradient *grad);


void draw_border(SDL_Surface *target, uint8_t thickness, t_color color);
