#include "minesweeper.h"
#include <logger.h>

t_coord *get_coord(int wx, int wy)
{
	t_coord *coord = calloc(1, sizeof(t_coord));
	int rx, ry, fw, fh;

	if (!coord)
	{
		console_error("Allocation error");
		exit(1);
	}
	coord->wx = wx;
	coord->wy = wy;

	rx = (wx - BORDER_WIDTH);
	ry = wy - 2 * BORDER_WIDTH - HEADER;
	fw = minesweeper.w - 2 * BORDER_WIDTH;
	fh = minesweeper.h - HEADER - 3 * BORDER_WIDTH;
	coord->x = rx / (fw / minesweeper.game.c);
	coord->y = ry / (fh / minesweeper.game.l);

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
		{
			tile->state = T_TRIGGERED_MINE;
			minesweeper.game.gstate = GS_END;
			minesweeper.game.state.type = E_DEAD;
		}
		else
			tile->state = tile->value ? T_NUMBER : T_CLICKED_NORMAL;
	}
}

void select_tile(t_coord *coord)
{
	t_tile *tile = minesweeper.game.map + coord->index;

	if (tile->state == T_FLAG)
		return;
	minesweeper.game.current_tile = coord;
	minesweeper.game.state.type = E_OH;
}

void mark_tile(t_coord *coord)
{
	minesweeper.game.current_tile = NULL;
	t_tile *tile = minesweeper.game.map + coord->index;
	int old = tile->state == T_FLAG;

	if (tile->state == T_NORMAL && minesweeper.game.remaining_mines)
		tile->state = T_FLAG;
	else if (tile->state == T_FLAG)
		tile->state = T_QUESTION_MARK;
	else if (tile->state -= T_NORMAL)
		tile->state = T_NORMAL;
	int new = tile->state == T_FLAG;

	minesweeper.game.remaining_mines += old - new;
}
