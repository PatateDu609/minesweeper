#include "minesweeper.h"

void init_header(void)
{
	t_header hdr = {};

	hdr.rect.x = hdr.rect.y = 0;
	hdr.rect.h = 30;

	minesweeper.hdr = hdr;
}
