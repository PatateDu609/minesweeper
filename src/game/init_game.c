#include "minesweeper.h"
#include "debug.h"
#include <unistd.h>

static int chk_mine(int *mines, int mine, int i_max)
{
	for (int i = 0; i < i_max; i++)
		if (mines[i] == mine)
			return 0;
	return 1;
}

static int *init_mines(t_game *game)
{
	int max = game->c * game->l;
	int *mines = malloc(sizeof(int) * game->m);

	for (int i = 0; i < game->m; i++)
	{
		int mine;
		do
			mine = rand() % max;
		while (!chk_mine(mines, mine, i));
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
			int x1 = x + j;
			int y1 = y + i;
			int coord = y1 * game->c + x1;

			if (0 <= x1 && x1 < game->c &&
				0 <= y1 && y1 < game->l &&
				!find_sorted(game->mines, game->m, coord))
				map[coord].value++;
		}
	}
}

static t_tile *create_field(t_game *game)
{
	int max = game->c * game->l;
	t_tile *map = calloc(max, sizeof(t_tile));
	int *mines = game->mines;

	for (int i = 0; i < max; i++)
	{
		map[i].hidden = 1;
		map[i].state = T_NORMAL;
	}

	for (int i = 0; i < game->m; i++)
	{
		int x = mines[i] % game->c;
		int y = mines[i] / game->c;
		int index = y * game->c + x;

		inc_around_mine(game, map, x, y);
		map[index].value = -1;
	}

	return map;
}

void start_game(t_game *game)
{
	game->seed = time(NULL) * getpid();
	console_log("seed = %d", game->seed);
	srand(game->seed);

	game->mines = init_mines(game);
	console_info("Mines initialization succeded");
	print_mines();

	game->map = create_field(game);
	console_info("Field initialization succeeded");
	print_field();
}

void init_field()
{
	console_info("Starting game initialization");

	t_game *game = &minesweeper.game;

	game->c = 9;
	game->l = 9;
	game->m = 10;
	game->mines = NULL;
	game->map = NULL;
	game->seed = 0;
}
