#include "ui/button.h"
#include "./button.h"

t_btn_group *new_btn_group(const size_t nb_btn, const size_t btn_space, const size_t start_x, const size_t start_y)
{
	t_btn_group *group = calloc(1, sizeof group);
	if (group == NULL)
		return group;

	if (nb_btn != 0)
	{
		group->btns = calloc(nb_btn, sizeof *group->btns);
		if (group->btns == NULL)
		{
			free(group);
			return NULL;
		}
	}

	group->btn_space = btn_space;
	group->max_btns  = nb_btn;
	group->nb_btns   = 0;
	group->x         = start_x;
	group->y         = start_y;
	group->direction = BTN_GRP_HORIZONTAL;

	return group;
}

uint8_t btn_group_append(t_btn_group *grp, const t_button button)
{
	if (grp->nb_btns == grp->max_btns)
	{
		void *tmp = realloc(grp->btns, ++grp->max_btns * sizeof *grp->btns);
		if (tmp == NULL)
			return 0;

		grp->btns = tmp;
	}

	if (grp->nb_btns == 0)
	{
		grp->btns[grp->nb_btns] = button;
		button->dst.x             = grp->x;
		button->dst.y             = grp->y;
		grp->nb_btns++;

		return 1;
	}

	size_t x, y;

	const t_button last_btn = grp->btns[grp->nb_btns - 1];
	if (grp->direction == BTN_GRP_HORIZONTAL)
	{
		x = last_btn->dst.x + last_btn->dst.w + grp->btn_space;
		y = grp->y;
	}
	else if (grp->direction == BTN_GRP_VERTICAL)
	{
		x = grp->x;
		y = last_btn->dst.y + last_btn->dst.h + grp->btn_space;
	}
	else
		abort();

	button->dst.x             = x;
	button->dst.y             = y;
	grp->btns[grp->nb_btns] = button;
	grp->nb_btns++;

	return 1;
}

static void btn_init(const t_button btn, const SDL_Rect rect, click_fn *on_click)
{
	btn->enabled = 1;

	btn->on_click = on_click;
	btn->dst      = rect;

	btn->on_hover         = NULL;
	btn->enable_hover_evt = 0;
}

t_button new_icon_btn(SDL_Texture *spritesheet, click_fn *on_click, const SDL_Rect src, const SDL_Rect dst)
{
	struct btn_icon *btn = calloc(1, sizeof *btn);
	if (btn == NULL)
		return NULL;

	btn->btn.type = ICON_BUTTON;
	btn->tex      = spritesheet;
	btn->src      = src;

	btn_init((t_button)btn, dst, on_click);

	return (t_button)btn;
}

t_button new_txt_btn(SDL_Rect rect, const char *txt, const FT_Face face, const t_color stroke, const t_color *bg, click_fn *on_click)
{
	struct btn_text *btn = calloc(1, sizeof *btn);
	if (btn == NULL)
		return NULL;

	btn->btn.type = TEXT_BUTTON;
	btn->txt      = new_text(txt, face);
	if (btn->txt == NULL)
	{
		free(btn);
		return NULL;
	}

	btn->color = stroke;
	btn->bg    = bg != NULL ? *bg : (t_color){.raw = 0};

	rect.x = 0;
	rect.y = 0;
	btn_init((t_button)btn, rect, on_click);

	return (t_button)btn;
}

void btn_free(const t_button button)
{
	if (button->type == ICON_BUTTON)
	{
		struct btn_icon *btn = (struct btn_icon*)button;
		SDL_DestroyTexture(btn->tex);
		free(btn);
	}
	else if (button->type == TEXT_BUTTON)
	{
		struct btn_text *btn = (struct btn_text*)button;
		free((char*)btn->txt->raw);
		free_text(btn->txt);
		free(btn);
	}
}

void btn_set_enabled(const t_button button, const uint8_t state)
{
	button->enabled = state;
}

void btn_should_update_on_hover(const t_button button, const uint8_t state)
{
	button->enable_hover_evt = state;
}

void btn_set_on_hover(const t_button button, hover_fn *fn)
{
	button->on_hover = fn;
}

void btn_icon_enable_alpha(const t_button button, const uint8_t state)
{
	if (button->type != ICON_BUTTON)
		return;

	((struct btn_icon*)button)->enable_alpha_check = state;
}

void btn_text_set_char_size(const t_button button, const float sz)
{
	if (button->type != TEXT_BUTTON)
		return;

	((struct btn_text*)button)->char_size = sz;
}

void btn_text_set_colors(const t_button button, const t_color *stroke, const t_color *bg)
{
	if (button->type != TEXT_BUTTON || (stroke == NULL && bg == NULL))
		return;

	struct btn_text *btn = (struct btn_text*)button;

	if (stroke != NULL)
		btn->color = *stroke;
	if (bg != NULL)
		btn->bg = *bg;
}

void btn_translate(const t_button btn, const ssize_t dx, const ssize_t dy)
{
	btn->dst.x += dx;
	btn->dst.y += dy;
}

void btn_set_coord(const t_button btn, const ssize_t x, const ssize_t y)
{
	btn->dst.x = x;
	btn->dst.y = y;
}
