#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <SDL2/SDL_ttf.h>

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

	t_color tile_number_color[8];
} t_sprites;

typedef struct
{
	t_emote_type type;
	SDL_Rect dst;
	SDL_bool clicked;
} t_state_emote;

typedef struct
{
	int l;
	int c;
	int m;

	uint32_t seed;
	uint32_t *mines;
	t_tile *map;

	time_t start_time;
	uint8_t remaining_mines;

	t_state_emote state;
	t_game_state gstate;
	time_t last_time;

	t_coord *current_tile;
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

	enum
	{
		BUTTON_NONE,
		BUTTON_RIGHT,
		BUTTON_LEFT,
	} clicked;
} t_minesweeper;

#endif
