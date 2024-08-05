#ifndef MINESWEEPER_COLOR_H
#define MINESWEEPER_COLOR_H

typedef union u_color
{
	Uint32 raw;


	struct
	{
		unsigned char a;
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};
} t_color;


#endif //MINESWEEPER_COLOR_H
