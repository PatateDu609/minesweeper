#include "minesweeper.h"
#include "logger/logger.h"

t_minesweeper minesweeper;

static void init_SDL(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
		consoleinfo("SDL initialization succeded");
	else
	{
		consoleerror("SDL initialization failed");
		exit(EXIT_FAILURE);
	}
}

static void init_window(void)
{
	minesweeper.window = SDL_CreateWindow(minesweeper.title,
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  minesweeper.w, minesweeper.h, 0);

	if (minesweeper.window)
		consoleinfo("Window creation succeded");
	else
	{
		consoleerror("Window creation failed");
		exit(EXIT_FAILURE);
	}
}

static void init_renderer(void)
{
	minesweeper.renderer = SDL_CreateRenderer(minesweeper.window, -1, SDL_RENDERER_ACCELERATED);

	if (minesweeper.renderer)
		consoleinfo("Renderer created successfully");
	else
	{
		consoleerror("Renderer was not created");
		exit(EXIT_FAILURE);
	}
}

void init(void)
{
	minesweeper.w = WIDTH;
	minesweeper.h = HEIGHT;
	minesweeper.title = WINDOW_TITLE;
	minesweeper.is_open = 1;

	init_SDL();
	init_window();
	init_renderer();
}

void free_all(void)
{
	SDL_DestroyRenderer(minesweeper.renderer);
	SDL_DestroyWindow(minesweeper.window);
	SDL_Quit();
}
