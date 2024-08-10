#include <assert.h>
#include <logger.h>

#include "ui.h"
#include "utils.h"

const t_linear_gradient grad_ui_header = {
	.nb_stops = 15,
	.stops = {
		(t_stop){
			.color = (t_color){.r = 0, .g = 88, .b = 238, .a = 255},
			.offset = 0,
		},

		(t_stop){
			.color = (t_color){.r = 53, .g = 147, .b = 255, .a = 255},
			.offset = .04f,
		},

		(t_stop){
			.color = (t_color){.r = 40, .g = 142, .b = 255, .a = 255},
			.offset = .06f,
		},

		(t_stop){
			.color = (t_color){.r = 18, .g = 125, .b = 255, .a = 255},
			.offset = .08f,
		},

		(t_stop){
			.color = (t_color){.r = 3, .g = 111, .b = 252, .a = 255},
			.offset = .10f,
		},

		(t_stop){
			.color = (t_color){.r = 2, .g = 98, .b = 238, .a = 255},
			.offset = .14f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 87, .b = 229, .a = 255},
			.offset = .20f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 84, .b = 227, .a = 255},
			.offset = .24f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 85, .b = 235, .a = 255},
			.offset = .56f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 91, .b = 245, .a = 255},
			.offset = .66f,
		},

		(t_stop){
			.color = (t_color){.r = 2, .g = 106, .b = 254, .a = 255},
			.offset = .76f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 98, .b = 239, .a = 255},
			.offset = .86f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 82, .b = 214, .a = 255},
			.offset = .92f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 64, .b = 171, .a = 255},
			.offset = .94f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 48, .b = 146, .a = 255},
			.offset = 1,
		},
	}
};

t_color apply_grad_pixel(const SDL_Rect rect, const SDL_Point pt, const struct linear_gradient *grad)
{
	if (pt.y <= rect.y)
		return grad->stops[0].color;
	if (rect.y + rect.h < pt.y)
		return grad->stops[grad->nb_stops - 1].color;

	const float        ratio = (float)(pt.y - rect.y) / (rect.h);
	const struct stop *start = NULL, *end = NULL;


	float prev = 0;
	for (size_t i = 0; i < grad->nb_stops; i++)
	{
		const float cur = grad->stops[i].offset;

		if (i != 0)
			assert(prev <= cur);
		else
			prev = cur;

		if (ratio <= cur)
		{
			assert(i > 0);
			end   = grad->stops + i;
			start = end - 1;
			break;
		}
	}

	assert(start != NULL);
	assert(end != NULL);

	const int start_stop  = rect.h * start->offset;
	const int stop_height = rect.h * end->offset - start_stop;
	const int ratio_r     = (end->color.r - start->color.r) / stop_height;
	const int ratio_g     = (end->color.g - start->color.g) / stop_height;
	const int ratio_b     = (end->color.b - start->color.b) / stop_height;

	t_color result;
	result.r = (int)start->color.r + (pt.y - start_stop) * ratio_r;
	result.g = (int)start->color.g + (pt.y - start_stop) * ratio_g;
	result.b = (int)start->color.b + (pt.y - start_stop) * ratio_b;
	result.a = 255;

	return result;
}


SDL_Texture *apply_grad(SDL_Renderer *renderer, const SDL_Rect rect, const struct linear_gradient *grad)
{
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
	t_color *pixels;
	int pitch;
	const int err = SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);
	if (err)
	{
		console_error("error: couldn't lock texture: %s", SDL_GetError());
		exit(1);
	}

	for (size_t y = 0; y < rect.h; y++)
	{
		const t_color col = apply_grad_pixel(rect, (SDL_Point){0, y}, grad);

		for (size_t x = 0; x < rect.w; x++)
			pixels[y * rect.w + x] = col;
	}

	SDL_UnlockTexture(texture);
	return texture;
}
