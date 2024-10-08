#pragma once

#include "ui/gradient.h"
#include "ui/button.h"

#include <stdint.h>
#include <SDL_ttf.h>


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


typedef enum
{
	GS_NONE,
	GS_INGAME,
	GS_END
} t_game_state;


typedef enum
{
	CTRL_INVALID = 0xFF,

	CTRL_CLOSE          = 0,
	CTRL_CLOSE_HOVER    = 1,
	CTRL_CLOSE_ACTIVE   = 2,
	CTRL_CLOSE_DISABLED = CTRL_INVALID,

	CTRL_MINIMIZE          = 4,
	CTRL_MINIMIZE_HOVER    = 5,
	CTRL_MINIMIZE_ACTIVE   = 6,
	CTRL_MINIMIZE_DISABLED = CTRL_INVALID,

	CTRL_MAXIMIZE          = 8,
	CTRL_MAXIMIZE_HOVER    = 9,
	CTRL_MAXIMIZE_ACTIVE   = 10,
	CTRL_MAXIMIZE_DISABLED = 11,
} t_controls;


typedef struct
{
	uint32_t index;
	int32_t  x;
	int32_t  y;

	int32_t wx;
	int32_t wy;
} t_coord;


typedef struct
{
	int          hidden;
	char         value;
	t_tile_types state;
} t_tile;


typedef struct
{
	SDL_Texture *texture_tiles;
	SDL_Rect     tiles[8];

	SDL_Texture *texture_emotes;
	SDL_Rect     emotes[5];

	SDL_Texture *texture_numbers_menu;
	SDL_Rect     numbers_menu[10];

	SDL_Texture *texture_numbers_tiles;
	SDL_Rect     numbers_tiles[8];

	SDL_Texture *borders;

	SDL_Texture *texture_controls;
	SDL_Rect     controls[12];
} t_sprites;


typedef struct
{
	t_emote_type type;
	SDL_Rect     dst;
	SDL_bool     clicked;
} t_state_emote;


typedef struct
{
	int l;
	int c;
	int m;

	uint32_t  seed;
	uint32_t *mines;
	t_tile *  map;

	time_t   start_time;
	uint16_t remaining_mines;
	uint32_t remaining_tiles;

	t_state_emote state;
	t_game_state  gstate;
	time_t        last_time;

	t_coord *current_tile;
} t_game;


struct header
{
	SDL_Rect rect;
	SDL_Rect title_rect;
	SDL_Rect icon_rect;

	SDL_Texture *icon;
	SDL_Texture *title;
	SDL_Texture *bg;

	t_btn_group *controls;
};


typedef struct header t_header;


typedef struct
{
	int   w;
	int   h;
	char *title;

	int win_width;
	int win_height;

	t_header      hdr;
	SDL_Window *  window;
	SDL_Renderer *renderer;

	int is_open;

	SDL_Color bg;
	t_color   game_hdr_color;
	TTF_Font *font;
	t_sprites sprites;

	SDL_Rect game_rect;
	t_game   game;


	enum
	{
		BUTTON_NONE,
		BUTTON_RIGHT,
		BUTTON_LEFT,
	} clicked;
} t_minesweeper;
