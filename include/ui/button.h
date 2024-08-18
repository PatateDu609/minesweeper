#pragma once

#include "ui.decl.h"

#include <stdint.h>
#include <stdlib.h>

#include <SDL_render.h>
#include <SDL_rect.h>
#include <SDL_events.h>

#include "../text.h"


struct button;


enum btn_group_dir
{
	BTN_GRP_HORIZONTAL,
	BTN_GRP_VERTICAL,
};


struct btn_group
{
	size_t x;
	size_t y;

	size_t    nb_btns;
	size_t    max_btns;
	t_button *btns;

	size_t btn_space;

	enum btn_group_dir direction;
};


t_btn_group *new_btn_group(size_t nb_btn, size_t btn_space, size_t start_x, size_t start_y);
uint8_t      btn_group_append(t_btn_group *grp, t_button button);
void         btn_group_render(SDL_Renderer *renderer, const t_btn_group *grp);

t_button new_icon_btn(SDL_Texture *spritesheet, click_fn *on_click, SDL_Rect src, SDL_Rect dst);
t_button new_txt_btn(SDL_Rect rect, const char *txt, FT_Face face, t_color stroke, const t_color *bg, click_fn *on_click);
void     btn_free(t_button button);

void btn_translate(t_button btn, ssize_t dx, ssize_t dy);
void btn_set_coord(t_button btn, ssize_t x, ssize_t y);

void btn_set_enabled(t_button button, uint8_t state);
void btn_should_update_on_hover(t_button button, uint8_t state);
void btn_set_on_hover(t_button button, hover_fn *fn);

void btn_icon_enable_alpha(t_button button, uint8_t state);
void btn_text_set_char_size(t_button button, float sz);
void btn_text_set_colors(t_button button, const t_color *stroke, const t_color *bg);

void btn_render(SDL_Renderer *renderer, t_button button);
void btn_process_evt(t_button button, t_mouse_evt evt);

t_mouse_evt get_event(SDL_Event *evt);
