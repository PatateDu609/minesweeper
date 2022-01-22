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
	if (minesweeper.game.current_tile)
	{
		t_tile *tile = minesweeper.game.map + minesweeper.game.current_tile->index;
		if (tile->state == T_CLICKED_NORMAL)
			tile->state = T_NORMAL;
		else if (tile->state == T_TRIGGERED_QUESTION_MARK)
			tile->state = T_QUESTION_MARK;
	}
	free(minesweeper.game.current_tile);
	minesweeper.game.current_tile = NULL;
	if (minesweeper.game.state.type != E_DEAD)
		minesweeper.game.state.type = E_NORMAL;
}

static void update_view()
{
	if (minesweeper.game.state.clicked)
		minesweeper.game.state.type = E_CLICKED_NORMAL;
	if (minesweeper.game.current_tile)
	{
		int index = minesweeper.game.current_tile->index;
		t_tile *tile = minesweeper.game.map + index;

		if (tile->state == T_NORMAL)
			tile->state = T_CLICKED_NORMAL;
		else if (tile->state == T_QUESTION_MARK)
			tile->state = T_TRIGGERED_QUESTION_MARK;
	}
}

static void mouse_event(int32_t x, int32_t y, uint8_t exist)
{
	reset_view();
	if (exist && minesweeper.clicked == BUTTON_NONE)
		return;

	uint8_t zone = get_click_zone(x, y);
	if (!zone)
		return;
	if (zone == 1 && minesweeper.game.gstate != GS_END)
	{
		t_coord *coord = get_coord(x, y);

		if (minesweeper.clicked == BUTTON_LEFT)
			select_tile(coord);
		else if (minesweeper.clicked == BUTTON_RIGHT)
			mark_tile(coord);
	}
	if (zone == 2)
		minesweeper.game.state.clicked = SDL_TRUE;
	update_view();
}

void mouse_click_down(SDL_MouseButtonEvent button)
{
	if (button.button == SDL_BUTTON_LEFT)
		minesweeper.clicked = BUTTON_LEFT;
	else if (button.button == SDL_BUTTON_RIGHT)
		minesweeper.clicked = BUTTON_RIGHT;
	mouse_event(button.x, button.y, 0);
}

void mouse_click_up(SDL_MouseButtonEvent __unused button)
{
	if (minesweeper.clicked == BUTTON_RIGHT)
	{
		minesweeper.clicked = BUTTON_NONE;
		return;
	}
	if (minesweeper.game.current_tile)
	{
		if (minesweeper.game.gstate == GS_NONE)
			start_game(&minesweeper.game, minesweeper.game.current_tile);
		flip(minesweeper.game.current_tile);
	}
	reset_view();
	update_view();

	minesweeper.clicked = BUTTON_NONE;
}

void mouse_move(SDL_MouseMotionEvent motion)
{
	mouse_event(motion.x, motion.y, 1);
}
