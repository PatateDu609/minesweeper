#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SDL2/SDL.h>

#include "defines.h"

typedef struct
{
	int w;
	int h;
	char *title;

	SDL_Window *window;
	SDL_Renderer *renderer;

	int is_open;
} t_minesweeper;

void init(void);
void free_all(void);

void launch(void);

extern t_minesweeper minesweeper;

#endif
