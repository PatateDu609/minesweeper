#include "defines.h"
#include "minesweeper.h"
#include "utils.h"
#include <logger.h>

static void draw_borders(void)
{
	SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.borders, NULL, NULL);
}

static void draw_emote()
{
	SDL_Rect src = minesweeper.sprites.emotes[minesweeper.game.state.type];
	SDL_Rect dst = minesweeper.game.state.dst;
	SDL_Texture *texture = minesweeper.sprites.texture_emotes;

	SDL_RenderCopy(minesweeper.renderer, texture, &src, &dst);
}

static void draw_digits_header(int digits, uint32_t nb, uint8_t side)
{
	t_score_numbers *arr = itoscores(digits, nb);
	SDL_Rect dst;
	float ratio = 304. / 544;
	int shift = .1 * HEADER;

	dst.h = .65 * HEADER;
	dst.w = ratio * dst.h;
	dst.x = side ? minesweeper.w - BORDER_WIDTH - shift - dst.w * digits : BORDER_WIDTH + shift;
	dst.y = BORDER_WIDTH + (HEADER - dst.h) / 2;

	for (int i = 0; i < digits; i++, dst.x += dst.w)
	{
		SDL_Rect src = minesweeper.sprites.numbers_menu[arr[i]];
		SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.texture_numbers_menu, &src, &dst);
	}

	free(arr);
}

static void draw_digit_tile(uint8_t value, SDL_Rect dst)
{
	SDL_Texture *tex = minesweeper.sprites.texture_numbers_tiles;
	SDL_Rect src = minesweeper.sprites.numbers_tiles[value];

	SDL_RenderCopy(minesweeper.renderer, tex, &src, &dst);
}

static void draw_header(void)
{
	uint32_t t;
	if (minesweeper.game.gstate == GS_NONE)
		t = 0;
	else if (minesweeper.game.gstate == GS_INGAME)
		t = time(NULL) - minesweeper.game.start_time;
	else
		t = minesweeper.game.last_time;
	minesweeper.game.last_time = t;

	draw_digits_header(3, minesweeper.game.remaining_mines, 0);
	draw_emote();
	draw_digits_header(4, t, 1);
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
			dst.x = x * WTILE + BORDER_WIDTH;
			dst.y = y * HTILE + HEADER + 2 * BORDER_WIDTH;

			if (!map[index].hidden && map[index].state == T_NUMBER)
				draw_digit_tile(map[index].value, dst);
			else
			{
				src = tiles[map[index].state];
				SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.texture_tiles, &src, &dst);
			}
		}
	}
}

void draw(void)
{
	draw_borders();
	draw_header();
	draw_field();
}
