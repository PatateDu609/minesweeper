#ifndef DEFINES_H
#define DEFINES_H

#define WIDTH 1160
#define HEIGHT 444
#define WINDOW_TITLE "minesweeper"
#define HEADER 60
#define BORDER_WIDTH 10

#define WTILE 32
#define HTILE 32

// BORDERS
#define NO_BORDER 0
#define LEFT_BORDER 0b00001
#define RIGHT_BORDER 0b00010
#define TOP_BORDER 0b00100
#define BOTTOM_BORDER 0b01000
#define HEADER_BORDER 0b10000

#define IS_LEFT_BORDER(x) (x & LEFT_BORDER)
#define IS_RIGHT_BORDER(x) (x & RIGHT_BORDER)
#define IS_TOP_BORDER(x) (x & TOP_BORDER)
#define IS_BOTTOM_BORDER(x) (x & BOTTOM_BORDER)
#define IS_HEADER_BORDER(x) (x & HEADER_BORDER)

#define LIGHT_SHADOW 0b10
#define NO_SHADOW 0b00
#define DARK_SHADOW 0b01

#define IS_LIGHT_SHADOW(x) (x & LIGHT_SHADOW)
#define IS_DARK_SHADOW(x) (x & DARK_SHADOW)

#define DARK_ZONE 0.20
#define LIGHT_ZONE 0.20

#endif
