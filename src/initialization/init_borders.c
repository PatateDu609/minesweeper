#include "defines.h"
#include "minesweeper.h"
#include <logger.h>

static uint8_t is_border(int x, int y)
{
	uint8_t ret = 0b00000;

	if (0 <= x && x <= BORDER_WIDTH)
		ret |= LEFT_BORDER;
	if (minesweeper.w - BORDER_WIDTH <= x && x <= minesweeper.w)
		ret |= RIGHT_BORDER;
	if (0 <= y && y <= BORDER_WIDTH)
		ret |= TOP_BORDER;
	if (minesweeper.h - BORDER_WIDTH <= y && y <= minesweeper.h)
		ret |= BOTTOM_BORDER;
	if (HEADER + BORDER_WIDTH <= y && y <= HEADER + 2 * BORDER_WIDTH)
		ret |= HEADER_BORDER;
	return ret;
}

static uint8_t compute_zone(int left, int right, int min, int max, int var)
{
	uint8_t ret = NO_SHADOW;

	if (min <= var && var <= left)
		ret |= LIGHT_SHADOW;
	if (right <= var && var <= max)
		ret |= DARK_SHADOW;
	return ret;
}

static uint8_t chk_overlap_header(uint8_t b, uint8_t s, uint8_t candidate)
{
	if (IS_LEFT_BORDER(b))
		return s;
	if (IS_RIGHT_BORDER(b))
		return s;
	return candidate;
}

static uint8_t chk_overlap_left(uint8_t b, uint8_t s, uint8_t candidate)
{
	if (!IS_DARK_SHADOW(candidate))
		return candidate;

	if (IS_HEADER_BORDER(b))
		return s;
	if (IS_TOP_BORDER(b))
		return s;
	if (IS_BOTTOM_BORDER(b))
		return s;
	return candidate;
}

static uint8_t chk_overlap_right(uint8_t b, uint8_t s, uint8_t candidate)
{
	if (!IS_LIGHT_SHADOW(candidate))
		return candidate;

	if (IS_HEADER_BORDER(b))
		return s;
	if (IS_TOP_BORDER(b))
		return s;
	if (IS_BOTTOM_BORDER(b))
		return s;
	return candidate;
}

static uint8_t chk_overlap_top(uint8_t b, uint8_t s, uint8_t candidate)
{
	if (!IS_DARK_SHADOW(candidate))
		return candidate;

	if (IS_RIGHT_BORDER(b))
		return s;
	if (IS_LEFT_BORDER(b))
		return s;
	return candidate;
}

static uint8_t chk_overlap_bottom(uint8_t b, uint8_t s, uint8_t candidate)
{
	if (!IS_LIGHT_SHADOW(candidate))
		return candidate;

	if (IS_RIGHT_BORDER(b))
		return s;
	if (IS_LEFT_BORDER(b))
		return s;
	return candidate;
}

static uint8_t select_zone(int x, int y, uint8_t b)
{
	uint8_t ret = NO_SHADOW;
	int left, right;
	int abs_l = BORDER_WIDTH * LIGHT_ZONE, abs_d = BORDER_WIDTH * (1 - DARK_ZONE);
	int min, max;

	if (IS_LEFT_BORDER(b))
	{
		min = 0;
		max = BORDER_WIDTH;
		left = abs_l;
		right = abs_d;
		ret |= chk_overlap_left(b, ret, compute_zone(left, right, min, max, x));
	}
	if (IS_TOP_BORDER(b))
	{
		min = 0;
		max = BORDER_WIDTH;
		left = abs_l;
		right = abs_d;
		ret |= chk_overlap_top(b, ret, compute_zone(left, right, min, max, y));
	}
	if (IS_RIGHT_BORDER(b) || IS_BOTTOM_BORDER(b))
	{
		max = minesweeper.w;
		min = max - BORDER_WIDTH;
		left = abs_l + min;
		right = abs_d + min;
		ret |= chk_overlap_right(b, ret, compute_zone(left, right, min, max, x));
	}
	if (IS_BOTTOM_BORDER(b))
	{
		max = minesweeper.h;
		min = max - BORDER_WIDTH;
		left = abs_l + min;
		right = abs_d + min;
		ret |= chk_overlap_bottom(b, ret, compute_zone(left, right, min, max, y));
	}
	if (IS_HEADER_BORDER(b))
	{
		min = HEADER + BORDER_WIDTH;
		max = HEADER + 2 * BORDER_WIDTH;
		left = abs_l + min;
		right = abs_d + min;
		ret |= chk_overlap_header(b, ret, compute_zone(left, right, min, max, y));
	}
	return ret;
}

t_color select_color(int x, int y)
{
	uint8_t b = is_border(x, y);
	if (!b)
		return (t_color){0};

	t_color light, color, dark, ret;
	color.r = color.g = color.b = 189;
	dark.r = dark.g = dark.b = 123;
	light.r = light.g = light.b = 255;
	light.a = dark.a = color.a = 0xff;

	ret = color;
	uint8_t s = select_zone(x, y, b);
	if (IS_LIGHT_SHADOW(s))
		ret = light;
	if (IS_DARK_SHADOW(s))
		ret = dark;

	return ret;
}
