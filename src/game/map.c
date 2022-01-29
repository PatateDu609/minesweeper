#include "minesweeper.h"
#include <logger.h>

static t_tile *_selected = NULL;
static t_coord *_coord;

int is_in_field(int x, int y)
{
	t_game *game = &minesweeper.game;

	return 0 <= x && x < game->c &&
		   0 <= y && y < game->l;
}

static void manage_highlighting(uint8_t mode)
{
	if (!_selected)
		return;

	for (int8_t i = -1; i <= 1; i++)
	{
		for (int8_t j = -1; j <= 1; j++)
		{
			if (!i && !j)
				continue;
			int32_t x = _coord->x + i;
			int32_t y = _coord->y + j;
			int32_t index = y * minesweeper.game.c + x;
			t_tile *current = minesweeper.game.map + index;

			if (is_in_field(x, y) && current->hidden)
				current->state = mode ? T_CLICKED_NORMAL : T_NORMAL;
		}
	}
}

void set_selected(t_coord *coord)
{
	t_coord *c = coord ? malloc(sizeof(t_coord)) : NULL;
	t_tile *tile = coord ? minesweeper.game.map + coord->index : NULL;

	manage_highlighting(0);
	if (!tile)
		free(_coord);
	else
		memcpy(c, coord, sizeof(t_coord));
	_coord = c;
	_selected = tile;
}

void highlight_selected(void)
{
	manage_highlighting(1);
}

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

static void game_finished()
{
	minesweeper.game.gstate = GS_END;
	minesweeper.game.state.type = E_COOL;
	minesweeper.game.remaining_mines = 0;

	for (int i = 0; i < minesweeper.game.m; i++)
	{
		t_tile *tile = minesweeper.game.map + minesweeper.game.mines[i];

		tile->state = T_FLAG;
	}
}

static void __flip(t_coord coord)
{
	t_tile *tile = minesweeper.game.map + coord.index;
	t_coord c;

	tile->state = T_NUMBER;
	if (tile->value >= 0 && tile->hidden)
		minesweeper.game.remaining_tiles--;
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

	if (tile->hidden)
	{
		tile->hidden = 0;
		if (tile->value >= 0)
			minesweeper.game.remaining_tiles--;
		if (tile->value == -1)
		{
			tile->state = T_TRIGGERED_MINE;
			game_lost();
		}
		else
			__flip(*coord);

		if (!minesweeper.game.remaining_tiles)
			game_finished();
	}
	else if (_selected)
		console_warn("Not supported yet");
}

void select_tile(t_coord *coord)
{
	t_tile *tile = minesweeper.game.map + coord->index;

	if (tile->state == T_FLAG)
		return;
	if (!tile->hidden)
		set_selected(coord);
	else
		set_selected(NULL);
	minesweeper.game.current_tile = coord;
	minesweeper.game.state.type = E_OH;
}

void mark_tile(t_coord *coord)
{
	minesweeper.game.current_tile = NULL;
	t_tile *tile = minesweeper.game.map + coord->index;
	int old = tile->state == T_FLAG;

	if (tile->state == T_NORMAL)
		tile->state = minesweeper.game.remaining_mines ? T_FLAG : T_QUESTION_MARK;
	else if (old)
		tile->state = T_QUESTION_MARK;
	else
		tile->state = T_NORMAL;
	int new = tile->state == T_FLAG;

	minesweeper.game.remaining_mines += old - new;
}
