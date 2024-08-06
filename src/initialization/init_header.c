#include "defines.h"
#include "minesweeper.h"


void init_header(void)
{
	t_header hdr = {};

	hdr.rect.x = hdr.rect.y = 0;
	hdr.rect.w = minesweeper.win_width;
	hdr.rect.h = HEIGHT_UI_HEADER;
	hdr.bg     = apply_grad(minesweeper.renderer, hdr.rect, &grad_ui_header);

	minesweeper.hdr = hdr;
}
