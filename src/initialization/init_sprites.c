#include "minesweeper.h"
#include <logger.h>

static void init_borders(void)
{
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;

	SDL_Surface *surface = SDL_CreateRGBSurface(0, minesweeper.w, minesweeper.h, 32,
												rmask, gmask, bmask, amask);

	for (int y = 0; y < minesweeper.h; y++)
		for (int x = 0; x < minesweeper.w; x++)
			set_pixel(surface, x, y, select_color(x, y));

	minesweeper.sprites.borders = SDL_CreateTextureFromSurface(minesweeper.renderer, surface);
	SDL_FreeSurface(surface);
}

static void init_map(void)
{
	SDL_Surface *spritesheet = IMG_Load("resources/map.png");
	if (spritesheet)
		console_info("Spritesheet loaded");
	else
		console_error("Spritesheet couldn't be loaded");

	minesweeper.sprites.texture_tiles = SDL_CreateTextureFromSurface(minesweeper.renderer, spritesheet);

	SDL_Rect rect = {.x = 0, .y = 0, .w = 512, .h = 512};
	for (t_tile_types i = 0; i < 8; i++)
	{
		rect.x = i * rect.w;
		minesweeper.sprites.tiles[i] = rect;
	}
	SDL_FreeSurface(spritesheet);
}

void init_sprites(void)
{
	init_borders();
	init_map();
}
