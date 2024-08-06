#include "ui.h"
#include "utils.h"

const t_linear_gradient grad_ui_header = {
	.nb_stops = 15,
	.stops = {
		(t_stop){
			.color = (t_color){.r = 0, .g = 88, .b = 238},
			.offset = 0,
		},

		(t_stop){
			.color = (t_color){.r = 53, .g = 147, .b = 255},
			.offset = .04f,
		},

		(t_stop){
			.color = (t_color){.r = 40, .g = 142, .b = 255},
			.offset = .06f,
		},

		(t_stop){
			.color = (t_color){.r = 18, .g = 125, .b = 255},
			.offset = .08f,
		},

		(t_stop){
			.color = (t_color){.r = 3, .g = 111, .b = 252},
			.offset = .10f,
		},

		(t_stop){
			.color = (t_color){.r = 2, .g = 98, .b = 238},
			.offset = .14f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 87, .b = 229},
			.offset = .20f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 84, .b = 227},
			.offset = .24f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 85, .b = 235},
			.offset = .56f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 91, .b = 245},
			.offset = .66f,
		},

		(t_stop){
			.color = (t_color){.r = 2, .g = 106, .b = 254},
			.offset = .76f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 98, .b = 239},
			.offset = .86f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 82, .b = 214},
			.offset = .92f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 64, .b = 171},
			.offset = .94f,
		},

		(t_stop){
			.color = (t_color){.r = 0, .g = 48, .b = 146},
			.offset = 1,
		},
	}
};



SDL_Texture *apply_grad(SDL_Renderer *renderer, const SDL_Rect rect, const struct linear_gradient *grad)
{
	const static uint32_t rmask   = 0xff000000;
	const static uint32_t gmask   = 0x00ff0000;
	const static uint32_t bmask   = 0x0000ff00;
	const static uint32_t amask   = 0x000000ff;
	SDL_Surface *         surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, rmask, gmask, bmask, amask);

	int *offsets = calloc(grad->nb_stops, sizeof *offsets);
	if (offsets == NULL)
		return NULL;

	for (size_t i  = 0; i < grad->nb_stops; i++)
		offsets[i] = grad->stops[i].offset * rect.h;

	const struct stop *start     = grad->stops, *end     = grad->stops + 1;
	const int *        start_off = offsets, *    end_off = offsets + 1;

	int ratio_r = (end->color.r - start->color.r) / (*end_off - *start_off);
	int ratio_g = (end->color.g - start->color.g) / (*end_off - *start_off);
	int ratio_b = (end->color.b - start->color.b) / (*end_off - *start_off);

	int start_y = 0;
	for (size_t y = 0; y < rect.h; y++)
	{
		int r;
		int g;
		int b;

		if (y > *end_off && end - grad->stops == grad->nb_stops)
		{
			r = end->color.r;
			g = end->color.g;
			b = end->color.b;
		}
		else
		{
			if (y > *end_off)
			{
				start++;
				start_off++;
				end++;
				end_off++;

				ratio_r = (end->color.r - start->color.r) / (*end_off - *start_off);
				ratio_g = (end->color.g - start->color.g) / (*end_off - *start_off);
				ratio_b = (end->color.b - start->color.b) / (*end_off - *start_off);

				start_y = 0;
			}

			r = (int)start->color.r + start_y * ratio_r;
			g = (int)start->color.g + start_y * ratio_g;
			b = (int)start->color.b + start_y * ratio_b;
			start_y++;
		}

		for (size_t x = 0; x < rect.w; x++)
			set_pixel(surface, x, y, (t_color){.r = r, .g = g, .b = b, .a = 255});
	}

	free(offsets);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return tex;
}
