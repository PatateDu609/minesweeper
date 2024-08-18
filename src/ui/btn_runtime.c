#include "ui/button.h"
#include "./button.h"
#include "./mouse_evt.h"

static void _btn_icon_render(SDL_Renderer *renderer, const struct btn_icon *icon)
{
	SDL_RenderCopy(renderer, icon->tex, &icon->src, &icon->btn.dst);
}

void btn_render(SDL_Renderer *renderer, const t_button button)
{
	if (button->type == ICON_BUTTON)
		_btn_icon_render(renderer, (struct btn_icon*)button);
}

void btn_process_evt(t_button button, t_mouse_evt evt) {}

void btn_group_render(SDL_Renderer *renderer, const struct btn_group *grp)
{
	for (size_t i = 0; i < grp->nb_btns; i++)
		btn_render(renderer, grp->btns[i]);
}
