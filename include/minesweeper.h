#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SDL2/SDL.h>

#include "defines.h"
#include "utils.h"

typedef struct
{
	int l;
	int c;
	int m;

	uint32_t seed;
	int *mines;
	char *map;
} t_game;

typedef struct
{
	int w;
	int h;
	char *title;

	SDL_Window *window;
	SDL_Renderer *renderer;

	int is_open;

	SDL_Color bg;
	t_game game;
} t_minesweeper;

void init(void);
void free_all(void);

void init_field(void);
void launch(void);

extern t_minesweeper minesweeper;

#endif
