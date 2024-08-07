#include "defines.h"
#include "minesweeper.h"
#include "utils.h"
#include <logger.h>

static void draw_ui(void)
{
	static uint8_t is_init = 0;
	static SDL_Rect icon_rect;
	static SDL_Rect title_rect;

	if (!is_init)
	{
		icon_rect.w = icon_rect.h = 28;
		icon_rect.x = minesweeper.win_width * 0.01;
		icon_rect.y = (HEIGHT_UI_HEADER - icon_rect.h) / 2 + icon_rect.h * 0.075;

		SDL_QueryTexture(minesweeper.hdr.title, NULL, NULL, &title_rect.w, &title_rect.h);

		const float ratio = (float)title_rect.w / (float)title_rect.h;
		const float max_h = HEIGHT_UI_HEADER * 0.50f;
		const float max_w = ratio * max_h;
		title_rect.w = max_w >= title_rect.w ? title_rect.w : (int)max_w;
		title_rect.h = max_h >= title_rect.h ? title_rect.h : (int)max_h;

		title_rect.x = icon_rect.x + icon_rect.w + 10;
		title_rect.y = (HEIGHT_UI_HEADER - title_rect.h) / 2 ;

		is_init = 1;
	}

	SDL_RenderCopy(minesweeper.renderer, minesweeper.hdr.bg, NULL, &minesweeper.hdr.rect);
	SDL_RenderCopy(minesweeper.renderer, minesweeper.hdr.icon, NULL, &icon_rect);
	SDL_RenderCopy(minesweeper.renderer, minesweeper.hdr.title, NULL, &title_rect);
}

static void draw_borders(void)
{
	SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.borders, NULL, &minesweeper.game_rect);
}

static void draw_emote()
{
	SDL_Rect     src     = minesweeper.sprites.emotes[minesweeper.game.state.type];
	SDL_Rect     dst     = minesweeper.game.state.dst;
	SDL_Texture *texture = minesweeper.sprites.texture_emotes;

	SDL_RenderCopy(minesweeper.renderer, texture, &src, &dst);
}

static void draw_digits_header(int digits, uint32_t nb, uint8_t side)
{
	t_score_numbers *arr = itoscores(digits, nb);
	SDL_Rect         dst;
	float            ratio = 304. / 544;
	int              shift = .1 * HEADER;

	dst.h = .65 * HEADER;
	dst.w = ratio * dst.h;
	dst.x = side ? minesweeper.w - BORDER_WIDTH - shift - dst.w * digits : BORDER_WIDTH + shift;
	dst.y = BORDER_WIDTH + (HEADER - dst.h) / 2;


	dst.x += WIDTH_UI_BORDERS;
	dst.y += HEIGHT_UI_HEADER;

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
	SDL_Rect     src = minesweeper.sprites.numbers_tiles[value];

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

	SDL_Rect rect;
	rect.x = WIDTH_UI_BORDERS;
	rect.y = HEIGHT_UI_HEADER;
	rect.w = minesweeper.w;
	rect.h = HEADER + (int)(BORDER_WIDTH * 1.1f);

	SDL_SetRenderDrawColor(minesweeper.renderer, minesweeper.game_hdr_color.r, minesweeper.game_hdr_color.g, minesweeper.game_hdr_color.b, minesweeper.game_hdr_color.a);
	SDL_RenderFillRect(minesweeper.renderer, &rect);

	draw_digits_header(3, minesweeper.game.remaining_mines, 0);
	draw_emote();
	draw_digits_header(4, t, 1);
}

static void draw_field(void)
{
	SDL_Rect  src, dst;
	SDL_Rect *tiles = minesweeper.sprites.tiles;
	int       l     = minesweeper.game.l, c = minesweeper.game.c;
	t_tile *  map   = minesweeper.game.map;

	const int base_x = WIDTH_UI_BORDERS;
	const int base_y = HEIGHT_UI_HEADER;

	dst.w = WTILE;
	dst.h = HTILE;

	for (int y = 0; y < l; y++)
	{
		for (int x = 0; x < c; x++)
		{
			int index = y * c + x;
			dst.x     = base_x + x * WTILE + BORDER_WIDTH;
			dst.y     = base_y + y * HTILE + HEADER + 2 * BORDER_WIDTH;

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
	draw_ui();
	draw_header();
	draw_field();
	draw_borders();
}
