#include "minesweeper.h"
#include "utils.h"
#include <logger.h>
#include <SDL_image.h>

static void init_borders(void)
{
	SDL_Surface *surface = create_rgb_surface(minesweeper.w, minesweeper.h);

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
		rect.x   = i * rect.w;
		rects[i] = rect;
	}
	SDL_FreeSurface(spritesheet);
}

void init_sprites(void)
{
	init_borders();
	load_spritesheet(
		&minesweeper.sprites.texture_tiles,
		minesweeper.sprites.tiles,
		8,
		"resources/map.png",
		512,
		512
	);
	load_spritesheet(
		&minesweeper.sprites.texture_emotes,
		minesweeper.sprites.emotes,
		5,
		"resources/emotes.png",
		512,
		512
	);
	load_spritesheet(
		&minesweeper.sprites.texture_numbers_menu,
		minesweeper.sprites.numbers_menu,
		10,
		"resources/numbers.png",
		304,
		544
	);
	load_spritesheet(
		&minesweeper.sprites.texture_numbers_tiles,
		minesweeper.sprites.numbers_tiles,
		8,
		"resources/tile_numbers.png",
		512,
		512
	);
}
