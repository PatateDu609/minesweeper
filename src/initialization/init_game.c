#include "defines.h"

#include "minesweeper.h"
#include "debug.h"
#include <unistd.h>

static int chk_mine(uint32_t *mines, uint32_t mine, int i_max)
{
	for (int i = 0; i < i_max; i++)
		if (mines[i] == mine)
			return 0;
	return 1;
}

static uint32_t *init_mines(t_game *game, t_coord *coord)
{
	int       max   = game->c * game->l;
	uint32_t *mines = malloc(sizeof(uint32_t) * game->m);

	for (int i = 0; i < game->m; i++)
	{
		uint32_t mine;
		do
			mine = rand() % max;
		while (mine != coord->index && !chk_mine(mines, mine, i));
		mines[i] = mine;
	}
	sort(mines, game->m);
	return mines;
}

static void inc_around_mine(t_game *game, t_tile *map, int x, int y)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			int x1    = x + j;
			int y1    = y + i;
			int coord = y1 * game->c + x1;

			if (is_in_field(x1, y1) &&
				!find_sorted(game->mines, game->m, coord))
				map[coord].value++;
		}
	}
}

static t_tile *create_field(t_game *game)
{
	int       max   = game->c * game->l;
	t_tile *  map   = calloc(max, sizeof(t_tile));
	uint32_t *mines = game->mines;

	for (int i = 0; i < max; i++)
	{
		map[i].hidden = 1;
		map[i].state  = T_NORMAL;
	}

	for (int i = 0; i < game->m; i++)
	{
		int x     = mines[i] % game->c;
		int y     = mines[i] / game->c;
		int index = y * game->c + x;

		inc_around_mine(game, map, x, y);
		map[index].value = -1;
	}

	return map;
}

void start_game(t_game *game, t_coord *start)
{
	game->seed = time(NULL) * getpid();
	console_log("seed = %d", game->seed);
	srand(game->seed);

	game->mines = init_mines(game, start);
	console_info("Mines initialization succeded");
	print_mines();

	free(game->map);
	game->map = create_field(game);
	console_info("Field initialization succeeded");
	print_field();

	game->start_time      = time(NULL);
	game->gstate          = GS_INGAME;
	game->state.type      = E_NORMAL;
	game->last_time       = game->start_time;
	game->remaining_mines = game->m;
	game->remaining_tiles = game->c * game->l - game->m;
}

void init_state(t_game *game)
{
	SDL_Rect dst;

	dst.w               = dst.h = 50;
	dst.x               = WIDTH_UI_BORDERS + (minesweeper.w - dst.w) / 2;
	dst.y               = HEIGHT_UI_HEADER + BORDER_WIDTH + (HEADER - dst.h) / 2;
	game->state.dst     = dst;
	game->state.type    = E_NORMAL;
	game->state.clicked = SDL_FALSE;
}

static t_tile *null_map_state(t_game *game)
{
	int     max = game->c * game->l;
	t_tile *map = calloc(max, sizeof(t_tile));

	for (int i = 0; i < max; i++)
	{
		map[i].state  = T_NORMAL;
		map[i].hidden = 1;
		map[i].value  = 0;
	}
	return map;
}

void init_field()
{
	console_info("Starting game initialization");

	t_game *game = &minesweeper.game;

	game->c               = 9;
	game->l               = 9;
	game->m               = 10;
	game->mines           = NULL;
	game->map             = null_map_state(game);
	game->gstate          = GS_NONE;
	game->seed            = 0;
	game->start_time      = 0;
	game->remaining_mines = game->m;
	game->current_tile    = NULL;
}

void reset_game(t_game *game)
{
	game->gstate = GS_NONE;

	free(game->map);
	free(game->mines);

	game->mines = NULL;
	game->map   = null_map_state(game);
}
