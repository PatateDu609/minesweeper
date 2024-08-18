#pragma once


#include "ui/ui.decl.h"

#include "text.h"
#include "color.h"


#include <SDL_rect.h>
#include <SDL_render.h>


enum btn_type
{
	TEXT_BUTTON,
	ICON_BUTTON,
};


struct button
{
	enum btn_type type;

	SDL_Rect  dst;
	uint8_t   enable_hover_evt;
	hover_fn *on_hover;
	click_fn *on_click;
	uint8_t   enabled;
};


struct btn_icon
{
	struct button btn;

	uint8_t      enable_alpha_check;
	SDL_Rect     src;
	SDL_Texture *tex;
};


struct btn_text
{
	struct button btn;

	struct text *txt;
	float        char_size;
	t_color      color;
	t_color      bg;
};
