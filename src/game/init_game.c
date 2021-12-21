#include "minesweeper.h"
#include "logger/logger.h"
#include <unistd.h>

int chk_mine(int *mines, int mine, int i_max)
{
	for (int i = 0; i < i_max; i++)
		if (mines[i] == mine)
			return 0;
	return 1;
}

int *init_mines(t_game *game)
{
	int max = game->c * game->l;
	int *mines = malloc(sizeof(int) * game->m);
	srand(game->seed);

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

void inc_around_mine(t_game *game, char *map, int x, int y)
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
				map[coord]++;
		}
	}
}

char *create_field(t_game *game)
{
	int max = game->c * game->l;
	char *map = calloc(max, sizeof(char));
	int *mines = game->mines;

	for (int i = 0; i < game->m; i++)
	{
		int x = mines[i] % game->c;
		int y = mines[i] / game->c;
		inc_around_mine(game, map, x, y);
		map[y * game->c + x] = -1;
	}

	return map;
}

void init_field()
{
	consoleinfo("Starting game initialization");

	t_game *game = &minesweeper.game;

	game->c = 10;
	game->l = 8;
	game->m = 10;
	game->seed = time(NULL) * getpid();

	char log[4096];
	sprintf(log, "seed = %d", game->seed);
	consoleinfo(log);

	game->mines = init_mines(game);
	consoleinfo("Mines initialization succeded");
	print_mines();

	game->map = create_field(game);
	consoleinfo("Field initialization succeeded");
	print_field();
}
