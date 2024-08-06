#include "minesweeper.h"
#include "defines.h"
#include <logger.h>

static uint8_t get_click_zone(int x, int y)
{
	static uint8_t  is_init = 0;
	static SDL_Rect field;

	if (!is_init)
	{
		field.x = minesweeper.game_rect.x + BORDER_WIDTH;
		field.y = minesweeper.game_rect.y + BORDER_WIDTH * 2 + HEADER;
		field.w = minesweeper.game_rect.w - BORDER_WIDTH * 2;
		field.h = minesweeper.game_rect.h - BORDER_WIDTH * 3 - HEADER; // removing bottom border as well

		is_init = 1;
	}

	const SDL_Point pt = {.x = x, .y = y};
	if (SDL_PointInRect(&pt, &minesweeper.game_rect)) // game guard
	{
		if (SDL_PointInRect(&pt, &minesweeper.game.state.dst))
			return 2;
		if (SDL_PointInRect(&pt, &field))
			return 1;
	}
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
	if (minesweeper.game.gstate != GS_END)
		minesweeper.game.state.type = E_NORMAL;
}

static void update_view()
{
	if (minesweeper.game.state.clicked)
		minesweeper.game.state.type = E_CLICKED_NORMAL;
	if (minesweeper.game.current_tile)
	{
		int     index = minesweeper.game.current_tile->index;
		t_tile *tile  = minesweeper.game.map + index;

		if (tile->state == T_NORMAL)
			tile->state = T_CLICKED_NORMAL;
		else if (tile->state == T_QUESTION_MARK)
			tile->state = T_TRIGGERED_QUESTION_MARK;
	}
}

static void emote_clicked()
{
	minesweeper.game.state.clicked = SDL_TRUE;

	reset_game(&minesweeper.game);
}

static void mouse_event(int32_t x, int32_t y, uint8_t exist)
{
	reset_view();
	if (exist && minesweeper.clicked == BUTTON_NONE)
		return;

	const uint8_t zone = get_click_zone(x, y);
	if (!zone)
		return;
	if (zone == 1 && minesweeper.game.gstate != GS_END)
	{
		t_coord *coord = get_coord(x - WIDTH_UI_BORDERS, y - HEIGHT_UI_HEADER);

		if (minesweeper.clicked == BUTTON_LEFT)
		{
			select_tile(coord);
			highlight_selected();
		}
		else if (minesweeper.clicked == BUTTON_RIGHT)
			mark_tile(coord);
	}
	if (zone == 2)
		emote_clicked();
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
	set_selected(NULL);
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
