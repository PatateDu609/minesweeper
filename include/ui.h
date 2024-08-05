#ifndef MINESWEEPER_UI_H
#define MINESWEEPER_UI_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL_ttf.h>

#include "color.h"

typedef void ( free_fn )(void *);

struct button;

typedef uint8_t ( hover_fn )(struct button*, size_t, size_t);
typedef uint8_t ( click_fn )(struct button*, size_t, size_t);


typedef struct button
{
	enum
	{
		TEXT_BUTTON,
		ICON_BUTTON,
	} type;


	SDL_Rect  rect;
	t_color   color;
	hover_fn *chk_hover;
	click_fn *chk_click;
	uint8_t enabled;


	union
	{
		struct
		{
			uint8_t      enable_alpha_check;
			SDL_Texture *tex;
		} icon;


		struct
		{
			char *       text;
			t_color      color;
			free_fn *    free_text;
			TTF_Font *   font;
			SDL_Texture *cached_tex;
		} text;
	};
} t_button;

typedef struct
{
	size_t x;
	size_t y;

	size_t nb_btns;
	t_button *btns;

	size_t btn_space;
} t_btn_group;

t_btn_group *new_btn_group(size_t nb_btn, size_t btn_space, size_t start_x, size_t start_y);
void btn_group_append(t_btn_group *btn, t_button *button);

#endif //MINESWEEPER_UI_H
