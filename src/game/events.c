#include "minesweeper.h"
#include <logger.h>

void click(SDL_MouseButtonEvent button)
{
	t_coord *coord = get_coord(button.x, button.y);
	console_log("coord.index = %d, console.x = %d, console.y = %d", coord->index, coord->x, coord->y);

	if (button.button == SDL_BUTTON_LEFT)
		!minesweeper.game.map ? start_game(&minesweeper.game) : flip(coord);

	free(coord);
}
