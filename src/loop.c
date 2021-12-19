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

static void update()
{
	SDL_UpdateWindowSurface(minesweeper.window);
}

void launch(void)
{
	SDL_Event e;
	while (minesweeper.is_open)
	{
		poll(&e);
		update();
	}
}
