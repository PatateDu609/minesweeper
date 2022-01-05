#include "minesweeper.h"
#include <logger.h>

static uint8_t get_click_zone(int x, int y)
{
	if (BORDER_WIDTH < x && x < minesweeper.w - BORDER_WIDTH &&
		2 * BORDER_WIDTH + HEADER < y && y < minesweeper.h - BORDER_WIDTH)
		return 1;
	SDL_Rect emote = minesweeper.game.state.dst;
	if (emote.x <= x && x <= emote.x + emote.w &&
		emote.y <= y && y <= emote.y + emote.h)
		return 2;
	return 0;
}

static void reset_view()
{
	minesweeper.game.state.clicked = SDL_FALSE;
	if (minesweeper.game.current_tile &&
		minesweeper.game.map[minesweeper.game.current_tile->index].state == T_CLICKED_NORMAL)
		minesweeper.game.map[minesweeper.game.current_tile->index].state = T_NORMAL;
	free(minesweeper.game.current_tile);
	minesweeper.game.current_tile = NULL;
}

static void update_view()
{
	if (minesweeper.game.state.clicked)
		minesweeper.game.state.type = E_CLICKED_NORMAL;
	else
		minesweeper.game.state.type = E_NORMAL;
	if (minesweeper.game.current_tile)
	{
		int index = minesweeper.game.current_tile->index;
		if (minesweeper.game.map[index].state == T_NORMAL)
			minesweeper.game.map[index].state = T_CLICKED_NORMAL;
	}
}

static void mouse_event(int32_t x, int32_t y, uint8_t exist)
{
	reset_view();
	if (exist && !minesweeper.clicked)
		return;

	uint8_t zone = get_click_zone(x, y);
	if (!zone)
		return;
	if (zone == 1)
	{
		t_coord *coord = get_coord(x, y);
		minesweeper.game.current_tile = coord;
	}
	if (zone == 2)
		minesweeper.game.state.clicked = SDL_TRUE;
	update_view();
}

void mouse_click_down(SDL_MouseButtonEvent button)
{
	minesweeper.clicked = SDL_TRUE;
	mouse_event(button.x, button.y, 0);
}

void mouse_click_up(SDL_MouseButtonEvent __unused button)
{
	if (minesweeper.game.current_tile)
	{
		minesweeper.game.gstate == GS_NONE ? start_game(&minesweeper.game)
										   : flip(minesweeper.game.current_tile);
	}

	reset_view();
	free(minesweeper.game.current_tile);
	minesweeper.game.current_tile = NULL;
	minesweeper.game.state.clicked = SDL_FALSE;
	update_view();
	minesweeper.clicked = SDL_FALSE;
}

void mouse_move(SDL_MouseMotionEvent motion)
{
	mouse_event(motion.x, motion.y, 1);
}
