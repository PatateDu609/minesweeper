#include "minesweeper.h"

static void poll(SDL_Event *event)
{
	while (SDL_PollEvent(event) > 0)
	{
		if (event->type == SDL_QUIT)
			minesweeper.is_open = 0;
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
			minesweeper.is_open = 0;
		if (event->type == SDL_MOUSEBUTTONDOWN)
			mouse_click_down(event->button);
		if (event->type == SDL_MOUSEBUTTONUP)
			mouse_click_up(event->button);
		if (event->type == SDL_MOUSEMOTION)
			mouse_move(event->motion);
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

void launch(void)
{
	static SDL_Event e;

	while (minesweeper.is_open)
	{
		clear();
		poll(&e);
		draw();
		update();
	}
}
