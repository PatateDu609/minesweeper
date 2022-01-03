#include "minesweeper.h"

static void draw_borders(void)
{
	SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.borders, NULL, NULL);
}

static void draw_field(void)
{
	SDL_Rect src, dst;
	SDL_Rect *tiles = minesweeper.sprites.tiles;
	int l = minesweeper.game.l, c = minesweeper.game.c;
	t_tile *map = minesweeper.game.map;

	dst.x = dst.y = 0;
	dst.w = WTILE;
	dst.h = HTILE;

	for (int y = 0; y < l; y++)
	{
		for (int x = 0; x < c; x++)
		{
			int index = y * c + x;

			src = map ? tiles[map[index].state] : tiles[T_NORMAL];

			dst.x = x * WTILE + BORDER_WIDTH;
			dst.y = y * HTILE + HEADER + 2 * BORDER_WIDTH;

			SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.texture_tiles, &src, &dst);
		}
	}
}

void draw(void)
{
	draw_borders();
	draw_field();
}
