#include <stdio.h>
#include <SDL2/SDL.h>
#include "minesweeper.h"

int main()
{
	init();
	launch();
	free_all();
	return 0;
}
