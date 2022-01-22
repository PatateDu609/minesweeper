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

static void game_lost()
{
	minesweeper.game.gstate = GS_END;
	minesweeper.game.state.type = E_DEAD;

	for (int i = 0; i < minesweeper.game.c * minesweeper.game.l; i++)
	{
		t_tile *tile = minesweeper.game.map + i;

		tile->hidden = 0;
		tile->state = (tile->value < 0) ? T_MINE : T_NUMBER;
	}
}

int is_in_field(int x, int y)
{
	t_game *game = &minesweeper.game;

	return 0 <= x && x < game->c &&
		   0 <= y && y < game->l;
}

static void __flip(t_coord coord)
{
	t_tile *tile = minesweeper.game.map + coord.index;
	t_coord c;

	tile->state = T_NUMBER;
	tile->hidden = 0;
	if (tile->value)
		return;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (!i && !j)
				continue;

			c.x = coord.x + j;
			c.y = coord.y + i;
			c.index = c.y * minesweeper.game.c + c.x;
			t_tile *to_check = minesweeper.game.map + c.index;

			if (is_in_field(c.x, c.y) && to_check->hidden)
				__flip(c);
		}
	}
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
			game_lost();
		}
		else
			__flip(*coord);
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
