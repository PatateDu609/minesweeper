#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "utils.h"

typedef enum
{
	T_NORMAL,
	T_CLICKED_NORMAL,
	T_FLAG,
	T_QUESTION_MARK,
	T_TRIGGERED_QUESTION_MARK,
	T_MINE,
	T_TRIGGERED_MINE,
	T_FALSE_MINE,
	T_NUMBER,
} t_tile_types;

typedef enum
{
	E_DEAD,
	E_COOL,
	E_OH,
	E_NORMAL,
	E_CLICKED_NORMAL,
} t_emote_type;

typedef enum
{
	SM_0,
	SM_1,
	SM_2,
	SM_3,
	SM_4,
	SM_5,
	SM_6,
	SM_7,
	SM_8,
	SM_9,
} t_score_numbers;

typedef union u_color
{
	Uint32 raw;
	struct
	{
		unsigned char a;
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};
} t_color;

typedef struct
{
	int index;
	int x;
	int y;

	int wx;
	int wy;
} t_coord;

typedef struct
{
	int hidden;
	char value;
	t_tile_types state;
} t_tile;

typedef struct
{
	SDL_Texture *texture_tiles;
	SDL_Rect tiles[8];

	SDL_Texture *texture_emotes;
	SDL_Rect emotes[5];

	SDL_Texture *texture_numbers_menu;
	SDL_Rect numbers_menu[10];

	SDL_Texture *borders;
} t_sprites;

typedef struct
{
	int l;
	int c;
	int m;

	uint32_t seed;
	int *mines;
	t_tile *map;
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
	TTF_Font *font;
	t_game game;
	t_sprites sprites;
} t_minesweeper;

void init(void);
void init_sprites(void);
void free_all(void);

void start_game(t_game *game);
void init_field(void);
void launch(void);

t_coord *get_coord(int wx, int wy);
void flip(t_coord *coord);

void click(SDL_MouseButtonEvent button);

void draw(void);

t_color select_color(int x, int y);

extern t_minesweeper minesweeper;

#endif
