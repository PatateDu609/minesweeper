#pragma once

#define WINDOW_TITLE "Minesweeper"
#define SIZE_MULTIPLIER 1
#define HEADER (70 * SIZE_MULTIPLIER)
#define BORDER_WIDTH (15 * SIZE_MULTIPLIER)

#define WTILE (48 * SIZE_MULTIPLIER)
#define HTILE (48 * SIZE_MULTIPLIER)

#define WIDTH_UI_BORDERS (5 * SIZE_MULTIPLIER)
#define HEIGHT_UI_HEADER (40 * SIZE_MULTIPLIER)

#define IS_CLAMPED(min, x, max) ((min) < (x) && (x) <= (max))
#define IS_CLAMPED_LAX(min, x, max) ((min) <= (x) && (x) <= (max))
