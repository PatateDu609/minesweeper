#include <SDL_image.h>

#include "defines.h"
#include "minesweeper.h"
#include "text.h"
#include "utils.h"


static SDL_Texture *get_window_title(t_header *hdr)
{
	SDL_Surface *surface = render_title(minesweeper.title, get_color(255, 255, 255, 255), hdr);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(minesweeper.renderer, surface);
	SDL_FreeSurface(surface);
	return tex;
}

static SDL_Texture *get_icon()
{
	SDL_Surface *icon = IMG_Load("resources/icon.png");

	SDL_Texture *tex = SDL_CreateTextureFromSurface(minesweeper.renderer, icon);
	SDL_FreeSurface(icon);
	return tex;
}


void init_header(void)
{
	t_header hdr = {};

	hdr.rect.x = hdr.rect.y = 0;
	hdr.rect.w = minesweeper.win_width;
	hdr.rect.h = HEIGHT_UI_HEADER;
	hdr.bg     = apply_grad(minesweeper.renderer, hdr.rect, &grad_ui_header);

	hdr.icon_rect.w = hdr.icon_rect.h = 28;
	hdr.icon_rect.x = minesweeper.win_width * 0.01;
	hdr.icon_rect.y = (HEIGHT_UI_HEADER - hdr.icon_rect.h) / 2 + hdr.icon_rect.h * 0.075;

	hdr.icon = get_icon();
	hdr.title = get_window_title(&hdr);

	minesweeper.hdr = hdr;
}
