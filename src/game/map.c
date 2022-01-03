#include "minesweeper.h"
#include <logger.h>

t_coord *get_coord(int wx, int wy)
{
	t_coord *coord = calloc(1, sizeof(t_coord));

	if (!coord)
	{
		console_error("Allocation error");
		exit(1);
	}
	coord->wx = wx;
	coord->wy = wy;
	coord->x = wx / (minesweeper.w / minesweeper.game.c);
	coord->y = wy / (minesweeper.h / minesweeper.game.l);
	coord->index = coord->y * minesweeper.game.c + coord->x;
	return coord;
}

void flip(t_coord *coord)
{
	t_tile *tile = minesweeper.game.map + coord->index;

	console_info("clicked tile : state = %d, value = %d, hidden = %d",
				 tile->state, tile->value, tile->hidden);
	if (tile->hidden)
	{
		tile->hidden = 0;
		if (tile->value == -1)
			tile->state = T_TRIGGERED_MINE;
	}
}
