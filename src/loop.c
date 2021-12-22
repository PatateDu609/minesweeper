#include "minesweeper.h"

static void poll(SDL_Event *event)
{
	while (SDL_PollEvent(event) > 0)
	{
		if (event->type == SDL_QUIT)
			minesweeper.is_open = 0;
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
			minesweeper.is_open = 0;
	}
}

static void clear()
{
	SDL_Color bg = minesweeper.bg;
	SDL_SetRenderDrawColor(minesweeper.renderer, bg.r, bg.g, bg.b, bg.a);
	SDL_RenderClear(minesweeper.renderer);
}

static void update()
{
	SDL_RenderPresent(minesweeper.renderer);
}

static void draw(void)
{
	SDL_Rect src, dst;

	dst.x = dst.y = 0;
	dst.w = dst.h = 32;

	for (t_tile_types i = 0; i < 8; i++)
	{
		src = minesweeper.sprites.tiles[i];
		dst.x = i * (dst.w + 16);
		SDL_RenderCopy(minesweeper.renderer, minesweeper.sprites.texture, &src, &dst);
	}
}

void launch(void)
{
	SDL_Event e;

	while (minesweeper.is_open)
	{
		clear();
		poll(&e);
		draw();
		update();
	}
}
