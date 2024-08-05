#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <logger.h>

#include "utils.h"

SDL_Texture *get_number(SDL_Renderer *renderer, TTF_Font *font, const int nb, const t_color c)
{
	const SDL_Color color   = get_sdl_color(c);
	const char *    al      = "12345678";
	SDL_Surface *   surface = TTF_RenderGlyph_Solid(font, al[nb - 1], color);
	SDL_Texture *   texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Rect center_text(SDL_Texture *tex, TTF_Font *font, const int glyph, SDL_Rect *dst)
{
	SDL_Rect       src;
	const SDL_Rect rect = *dst;
	const float    r    = 15;

	int minx, maxx;
	int advance;

	TTF_GlyphMetrics(font, "12345678"[glyph - 1], &minx, &maxx, NULL, NULL, &advance);

	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);
	dst->w = src.w * r;
	dst->h = src.h * r;

	dst->x = rect.x + (advance - maxx + minx) * r + (rect.w - dst->w) / 2;
	dst->y = rect.y + (rect.h - dst->h) / 2;

	return src;
}

void render_nb(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *texture, SDL_Texture __unused *numbers[8], const int h)
{
	SDL_Rect dst;

	dst.x = 0;
	dst.y = 0;
	dst.w = dst.h = h;

	SDL_RenderClear(renderer);
	for (int i = 0; i < 9; i++, dst.x += dst.w)
	{
		SDL_RenderCopy(renderer, texture, NULL, &dst);
		if (i)
		{
			const SDL_Rect _dst = dst;
			SDL_Rect       src  = center_text(numbers[i - 1], font, i, &dst);

			SDL_RenderCopy(renderer, numbers[i - 1], &src, &dst);
			dst = _dst;
		}
	}
	SDL_RenderPresent(renderer);
}

void save_renderer(const char *file_name, SDL_Renderer *renderer)
{
	int width, height;

	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, file_name);
	SDL_FreeSurface(surface);
}

int main()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		return 1;
	console_info("sdl_inited...");
	if (!IMG_Init(IMG_INIT_PNG))
		return 1;
	console_info("img_inited...");
	if (TTF_Init())
		return 1;
	console_info("ttf_inited...");

	t_color c[8];

	c[0] = get_color(0, 128, 0, 255);
	c[1] = get_color(0, 0, 255, 255);
	c[2] = get_color(255, 0, 0, 255);
	c[3] = get_color(0, 0, 128, 255);
	c[4] = get_color(128, 0, 0, 255);
	c[5] = get_color(0, 128, 128, 255);
	c[6] = get_color(0, 0, 0, 255);
	c[7] = get_color(128, 128, 128, 255);

	SDL_Window *  window   = SDL_CreateWindow("Tile Numbers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512 * 9, 512, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	TTF_Font *font = TTF_OpenFont("resources/Minecraftia-Regular.ttf", 24);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	SDL_Texture *numbers[8];
	for (int i     = 0; i < 8; i++)
		numbers[i] = get_number(renderer, font, i + 1, c[i]);

	const static uint32_t rmask = 0xff000000;
	const static uint32_t gmask = 0x00ff0000;
	const static uint32_t bmask = 0x0000ff00;
	const static uint32_t amask = 0x000000ff;
	SDL_Surface *         res   = SDL_CreateRGBSurface(0, 512, 512, 32, rmask, gmask, bmask, amask);

	SDL_Surface *  spritesheet = IMG_Load("resources/map.png");
	const SDL_Rect src         = {.x = 512, .y = 0, .w = 512, .h = 512};
	SDL_BlitSurface(spritesheet, &src, res, NULL);
	SDL_FreeSurface(spritesheet);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, res);

	SDL_Event e;
	int       is_open = 1;
	int       clicked = 0;
	while (is_open)
	{
		while (SDL_PollEvent(&e))
		{
			clicked = 0;
			if (e.type == SDL_QUIT)
				is_open = 0;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				clicked = 1;
		}

		render_nb(renderer, font, texture, numbers, 512);
		if (clicked)
		{
			const char *file_target __unused = "resources/tile_numbers.png";
			save_renderer(file_target, renderer);
			break;
		}
	}

	for (int i = 0; i < 8; i++)
		SDL_DestroyTexture(numbers[i]);
	TTF_CloseFont(font);
	SDL_FreeSurface(res);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
