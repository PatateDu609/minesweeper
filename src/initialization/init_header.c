#include <SDL_image.h>

#include "defines.h"
#include "minesweeper.h"


static SDL_Texture *get_window_title()
{
	TTF_Font *font = TTF_OpenFont("resources/NotoSans.ttf", 35);
	char *    dup  = strdup(minesweeper.title);
	dup[0]         = toupper(dup[0]);

	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, dup, (SDL_Color){255, 255, 255, 255});

	TTF_CloseFont(font);
	free(dup);

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
	hdr.title  = get_window_title();
	hdr.icon   = get_icon();

	minesweeper.hdr = hdr;
}
