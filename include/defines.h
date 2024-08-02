#ifndef DEFINES_H
#define DEFINES_H

#define WINDOW_TITLE "minesweeper"
#define SIZE_MULTIPLIER 1
#define HEADER (70 * SIZE_MULTIPLIER)
#define BORDER_WIDTH (15 * SIZE_MULTIPLIER)

#define WTILE (48 * SIZE_MULTIPLIER)
#define HTILE (48 * SIZE_MULTIPLIER)

#define IS_CLAMPED(min, x, max) ((min) < (x) && (x) <= (max))

#endif
