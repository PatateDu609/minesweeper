#include "minesweeper.h"
#include <logger.h>

t_minesweeper minesweeper;

static void init_SDL(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
		console_info("SDL initialization succeded");
	else
	{
		console_error("SDL initialization failed");
		exit(EXIT_FAILURE);
	}

	if (IMG_Init(IMG_INIT_PNG))
		console_info("SDL_image initialization succeded");
	else
	{
		console_error("SDL_image initialization failed");
		exit(EXIT_FAILURE);
	}

	if (!TTF_Init())
		console_info("SDL_TTF initialization succeded");
	else
	{
		console_error("SDL_TTF initialization failed");
		exit(EXIT_FAILURE);
	}
}

static void init_window(void)
{
	minesweeper.window = SDL_CreateWindow(minesweeper.title,
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  minesweeper.w, minesweeper.h,
										  SDL_WINDOW_SHOWN);

	if (minesweeper.window)
		console_info("Window creation succeded");
	else
	{
		console_error("Window creation failed");
		exit(EXIT_FAILURE);
	}
}

static void init_renderer(void)
{
	minesweeper.renderer = SDL_CreateRenderer(minesweeper.window, -1, SDL_RENDERER_ACCELERATED);

	if (minesweeper.renderer)
		console_info("Renderer created successfully");
	else
	{
		console_error("Renderer was not created");
		exit(EXIT_FAILURE);
	}
}

static void init_font(void)
{
	minesweeper.font = TTF_OpenFont("resources/Minecraftia-Regular.ttf", 24);

	if (minesweeper.font)
		console_info("Font loaded.");
}

void init(void)
{
	init_field();

	minesweeper.w = WTILE * minesweeper.game.c + 2 * BORDER_WIDTH;
	minesweeper.h = HTILE * minesweeper.game.l + HEADER + BORDER_WIDTH * 3;
	minesweeper.title = WINDOW_TITLE;
	minesweeper.is_open = 1;
	minesweeper.bg = (SDL_Color){.r = 189, .g = 189, .b = 189, .a = 255};
	minesweeper.clicked = SDL_FALSE;

	init_SDL();
	init_window();
	init_renderer();
	init_sprites();
	init_font();

	init_state(&minesweeper.game);
}

void free_all(void)
{
	free(minesweeper.game.mines);
	free(minesweeper.game.map);

	SDL_DestroyRenderer(minesweeper.renderer);
	SDL_DestroyWindow(minesweeper.window);
	IMG_Quit();
	SDL_Quit();
}
