#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "defines.h"
#include "utils.h"

typedef enum tile_types
{
	NORMAL,
	CLICKED_NORMAL,
	FLAG,
	QUESTION_MARK,
	TRIGGERED_QUESTION_MARK,
	MINE,
	TRIGGERED_MINE,
	FALSE_MINE,
	NUMBER,
} t_tile_types;

typedef struct
{
	SDL_Texture *texture;

	SDL_Rect tiles[8];
} t_sprites;

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
	t_sprites sprites;
} t_minesweeper;

void init(void);
void free_all(void);

void init_field(void);
void launch(void);

extern t_minesweeper minesweeper;

#endif
