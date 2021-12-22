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

	if (IMG_Init(IMG_INIT_PNG))
		consoleinfo("SDL_image initialization succeded");
	else
	{
		consoleerror("SDL_imaage initialization failed");
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

static void init_sprites(void)
{
	SDL_Surface *spritesheet = IMG_Load("resources/map.png");
	if (spritesheet)
	{
		char msg[1024];
		sprintf(msg, "Spritesheet loaded. Dimensions : w = %d, h = %d", spritesheet->w, spritesheet->h);
		consoleinfo(msg);
	}
	else
		consoleerror("Spritesheet couldn't be loaded");

	minesweeper.sprites.texture = SDL_CreateTextureFromSurface(minesweeper.renderer, spritesheet);

	SDL_Rect rect = {.x = 0, .y = 0, .w = 512, .h = 512};
	for (t_tile_types i = 0; i < 8; i++)
	{
		rect.x = i * rect.w;
		minesweeper.sprites.tiles[i] = rect;
	}
	SDL_FreeSurface(spritesheet);
}

void init(void)
{
	minesweeper.w = WIDTH;
	minesweeper.h = HEIGHT;
	minesweeper.title = WINDOW_TITLE;
	minesweeper.is_open = 1;
	minesweeper.bg = (SDL_Color){.r = 128, .g = 128, .b = 128, .a = 255};

	init_field();
	init_SDL();
	init_window();
	init_renderer();
	init_sprites();
}

void free_all(void)
{
	SDL_DestroyRenderer(minesweeper.renderer);
	SDL_DestroyWindow(minesweeper.window);
	IMG_Quit();
	SDL_Quit();
}
