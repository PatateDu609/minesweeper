#include "logger/logger.h"
#include "minesweeper.h"

void print_mines()
{
	int *mines = minesweeper.game.mines;
	char *result = strdup("");

	for (int i = 0; i < minesweeper.game.m; i++)
	{
		char nb[32], *tmp = result;
		sprintf(nb, (i + 1 < minesweeper.game.m) ? "%d " : "%d", mines[i]);
		result = strjoin(result, nb);
		free(tmp);
	}
	consolelog(result);
}

void print_field()
{
	char *map = minesweeper.game.map;
	char *line;

	for (int l = 0; l < minesweeper.game.l; l++)
	{
		line = strdup("");

		for (int c = 0; c < minesweeper.game.c; c++)
		{
			char *tmp = line, nb[14];
			char val = map[l * minesweeper.game.c + c];
			if (c + 1 == minesweeper.game.c)
				sprintf(nb, (val >= 0) ? " %d" : "%d", val);
			else
				sprintf(nb, (val >= 0) ? " %d " : "%d ", val);
			line = strjoin(line, nb);
			free(tmp);
		}
		consolelog(line);
	}
}
