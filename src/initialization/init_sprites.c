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

static void load_spritesheet(SDL_Texture **target, SDL_Rect *rects, int nb, char *path, int w, int h)
{
	SDL_Surface *spritesheet = IMG_Load(path);
	if (spritesheet)
		console_info("%s loaded", path);
	else
		console_error("%s couldn't be loaded", path);

	*target = SDL_CreateTextureFromSurface(minesweeper.renderer, spritesheet);

	SDL_Rect rect = {.x = 0, .y = 0, .w = w, .h = h};
	for (int i = 0; i < nb; i++)
	{
		rect.x = i * rect.w;
		rects[i] = rect;
	}
	SDL_FreeSurface(spritesheet);
}

static void init_colors(void)
{
	minesweeper.sprites.tile_number_color[1] = get_color(0, 128, 0, 255);
	minesweeper.sprites.tile_number_color[0] = get_color(0, 0, 255, 255);
	minesweeper.sprites.tile_number_color[2] = get_color(255, 0, 0, 255);
	minesweeper.sprites.tile_number_color[3] = get_color(0, 0, 128, 255);
	minesweeper.sprites.tile_number_color[4] = get_color(128, 0, 0, 255);
	minesweeper.sprites.tile_number_color[5] = get_color(0, 128, 128, 255);
	minesweeper.sprites.tile_number_color[6] = get_color(0, 0, 0, 255);
	minesweeper.sprites.tile_number_color[7] = get_color(128, 128, 128, 255);
}

void init_sprites(void)
{
	init_borders();
	load_spritesheet(&minesweeper.sprites.texture_tiles,
					 minesweeper.sprites.tiles, 8,
					 "resources/map.png", 512, 512);
	load_spritesheet(&minesweeper.sprites.texture_emotes,
					 minesweeper.sprites.emotes, 5,
					 "resources/emotes.png", 512, 512);
	load_spritesheet(&minesweeper.sprites.texture_numbers_menu,
					 minesweeper.sprites.numbers_menu, 10,
					 "resources/numbers.png", 304, 544);
	init_colors();
}
