#include <assert.h>

#include "defines.h"
#include "minesweeper.h"
#include "borders_utils.h"

#include <logger.h>

static struct borders_settings       settings;
static struct border_conflict_solver conflict_solvers[6];

static void func_from_points(float *m, float *p, const int x1, const int y1, const int x2, const int y2)
{
	if (m == NULL || p == NULL)
	{
		fprintf(stderr, "error: expecting pointers m (%p) and p (%p) to be non-null\n", m, p);
		abort();
	}

	*m = (float)(y2 - y1) / (float)(x2 - x1);
	*p = y1 - *m * x1;
}

static void init_conflict_solver(const enum border_region region, const int32_t idx)
{
	int                min_x = -1,           max_x = -1, min_y = -1, max_y = -1;
	enum border_shadow right = WEIRD_SHADOW, left  = WEIRD_SHADOW;

	if (region & BORDER_END_LEFT)
	{
		min_x = settings.left_border_no_shadow_end;
		max_x = settings.left_border_end;
		left  = DARK_SHADOW;
	}
	else if (region & BORDER_START_LEFT)
	{
		min_x = settings.left_border_start;
		max_x = settings.left_border_no_shadow_start;
		left  = LIGHT_SHADOW;
	}
	else if (region & BORDER_END_RIGHT)
	{
		min_x = settings.right_border_no_shadow_end;
		max_x = settings.right_border_end;
		right = DARK_SHADOW;
	}
	else if (region & BORDER_START_RIGHT)
	{
		min_x = settings.right_border_start;
		max_x = settings.right_border_no_shadow_start;
		right = LIGHT_SHADOW;
	}

	if (region & BORDER_START_TOP)
	{
		min_y = settings.top_border_start;
		max_y = settings.top_border_no_shadow_start;
		left  = LIGHT_SHADOW;
	}
	else if (region & BORDER_END_TOP)
	{
		min_y = settings.top_border_no_shadow_end;
		max_y = settings.top_border_end;
		left  = DARK_SHADOW;
	}
	else if (region & BORDER_START_INTERNAL)
	{
		min_y = settings.header_border_start;
		max_y = settings.header_border_no_shadow_start;
		right = LIGHT_SHADOW;
	}
	else if (region & BORDER_END_INTERNAL)
	{
		min_y = settings.header_border_no_shadow_end;
		max_y = settings.header_border_end;
		left  = DARK_SHADOW;
	}
	else if (region & BORDER_START_BOTTOM)
	{
		min_y = settings.bottom_border_start;
		max_y = settings.bottom_border_no_shadow_start;
		right = LIGHT_SHADOW;
	}
	else if (region & BORDER_END_BOTTOM)
	{
		min_y = settings.bottom_border_no_shadow_end;
		max_y = settings.bottom_border_end;
		right = DARK_SHADOW;
	}

	assert(min_x >= 0 && min_y >= 0 && max_x >= 0 && max_y >= 0);
	assert(left != WEIRD_SHADOW && right != WEIRD_SHADOW);

	conflict_solvers[idx].region       = region;
	conflict_solvers[idx].left_shadow  = left;
	conflict_solvers[idx].right_shadow = right;
	func_from_points(&conflict_solvers[idx].m, &conflict_solvers[idx].p, min_x, max_y, max_x, min_y);
}

static void init_settings()
{
	static uint8_t done = 0;
	if (done)
		return;
	done = 1;

	static const int abs_d = BORDER_WIDTH * BORDER_DARK_ZONE, abs_l = BORDER_WIDTH * BORDER_LIGHT_ZONE;

	settings.left_border_start           = 0;
	settings.left_border_no_shadow_start = abs_l;
	settings.left_border_no_shadow_end   = BORDER_WIDTH - abs_d;
	settings.left_border_end             = BORDER_WIDTH;

	settings.top_border_start           = 0;
	settings.top_border_no_shadow_start = abs_l;
	settings.top_border_no_shadow_end   = BORDER_WIDTH - abs_d;
	settings.top_border_end             = BORDER_WIDTH;

	settings.right_border_start           = minesweeper.w - BORDER_WIDTH;
	settings.right_border_no_shadow_start = settings.right_border_start + abs_l;
	settings.right_border_no_shadow_end   = minesweeper.w - abs_d;
	settings.right_border_end             = minesweeper.w;

	settings.bottom_border_start           = minesweeper.h - BORDER_WIDTH;
	settings.bottom_border_no_shadow_start = settings.bottom_border_start + abs_l;
	settings.bottom_border_no_shadow_end   = minesweeper.h - abs_d;
	settings.bottom_border_end             = minesweeper.h;

	settings.header_border_start           = settings.left_border_end + HEADER;
	settings.header_border_no_shadow_start = settings.header_border_start + abs_l;
	settings.header_border_end             = settings.header_border_start + BORDER_WIDTH;
	settings.header_border_no_shadow_end   = settings.header_border_end - abs_d;

	settings.weird_shadow = (t_color){.r = 255, .g = 87, .b = 51, .a = 255};
	settings.no_shadow    = (t_color){.r = 189, .g = 189, .b = 189, .a = 0xff};
	settings.light_shadow = (t_color){.r = 255, .g = 255, .b = 255, .a = 0xff};
	settings.dark_shadow  = (t_color){.r = 123, .g = 123, .b = 123, .a = 0xff};

	init_conflict_solver(BORDER_START_TOP | BORDER_END_RIGHT, 0);
	init_conflict_solver(BORDER_END_TOP | BORDER_START_RIGHT, 1);
	init_conflict_solver(BORDER_END_LEFT | BORDER_START_INTERNAL, 2);
	init_conflict_solver(BORDER_START_RIGHT | BORDER_END_INTERNAL, 3);
	init_conflict_solver(BORDER_END_LEFT | BORDER_START_BOTTOM, 4);
	init_conflict_solver(BORDER_START_LEFT | BORDER_END_BOTTOM, 5);
}

static uint8_t is_dark_simple_case(const enum border_region region)
{
	if (region & BORDER_END_RIGHT && !(region & BORDER_START_TOP))
		return 1;
	if (region & BORDER_END_BOTTOM && !(region & BORDER_START_LEFT))
		return 1;
	if (region & BORDER_END_TOP && !(region & (BORDER_START_LEFT | BORDER_START_RIGHT)))
		return 1;
	if (region & BORDER_END_LEFT && !(region & (BORDER_START_TOP | BORDER_START_INTERNAL | BORDER_START_BOTTOM)))
		return 1;
	if (region & BORDER_END_INTERNAL && !(region & (BORDER_START_LEFT | BORDER_START_RIGHT)))
		return 1;

	return 0;
}

static uint8_t is_light_simple_case(const enum border_region region)
{
	if (region & BORDER_START_TOP && !(region & BORDER_END_RIGHT))
		return 1;
	if (region & BORDER_START_RIGHT && !(region & (BORDER_END_TOP | BORDER_END_INTERNAL)))
		return 1;
	if (region & BORDER_START_LEFT && !(region & BORDER_END_BOTTOM))
		return 1;
	if (region & BORDER_START_BOTTOM && !(region & BORDER_END_LEFT))
		return 1;
	if (region & BORDER_START_INTERNAL && !(region & (BORDER_END_LEFT | BORDER_END_RIGHT)))
		return 1;

	return 0;
}

static enum border_shadow get_border_shadow_by_region(const enum border_region region)
{
	const static uint32_t MIDDLE_MASK           = BORDER_MIDDLE_TOP | BORDER_MIDDLE_BOTTOM | BORDER_MIDDLE_LEFT | BORDER_MIDDLE_RIGHT | BORDER_MIDDLE_INTERNAL;
	const static uint32_t EXTERNAL_SHADOWS_MASK = BORDER_START_TOP | BORDER_END_BOTTOM | BORDER_START_LEFT | BORDER_END_RIGHT;

	if (!(region & EXTERNAL_SHADOWS_MASK) && region & MIDDLE_MASK)
		return region & MIDDLE_MASK ? NO_SHADOW : WEIRD_SHADOW;

	if (is_dark_simple_case(region))
		return DARK_SHADOW;
	if (is_light_simple_case(region))
		return LIGHT_SHADOW;

	return WEIRD_SHADOW;
}

static enum border_region find_border_region(const int x, const int y)
{
	enum border_region regions = NO_BORDER;

	if (IS_CLAMPED(settings.left_border_start, x, settings.left_border_no_shadow_start))
		regions |= BORDER_START_LEFT;
	else if (IS_CLAMPED(settings.left_border_no_shadow_start, x, settings.left_border_no_shadow_end))
		regions |= BORDER_MIDDLE_LEFT;
	else if (IS_CLAMPED(settings.left_border_no_shadow_end, x, settings.left_border_end))
		regions |= BORDER_END_LEFT;

	if (IS_CLAMPED(settings.top_border_start, y, settings.top_border_no_shadow_start))
		regions |= BORDER_START_TOP;
	else if (IS_CLAMPED(settings.top_border_no_shadow_start, y, settings.top_border_no_shadow_end))
		regions |= BORDER_MIDDLE_TOP;
	else if (IS_CLAMPED(settings.top_border_no_shadow_end, y, settings.top_border_end))
		regions |= BORDER_END_TOP;

	if (IS_CLAMPED(settings.right_border_start, x, settings.right_border_no_shadow_start))
		regions |= BORDER_START_RIGHT;
	else if (IS_CLAMPED(settings.right_border_no_shadow_start, x, settings.right_border_no_shadow_end))
		regions |= BORDER_MIDDLE_RIGHT;
	else if (IS_CLAMPED(settings.right_border_no_shadow_end, x, settings.right_border_end))
		regions |= BORDER_END_RIGHT;

	if (IS_CLAMPED(settings.bottom_border_start, y, settings.bottom_border_no_shadow_start))
		regions |= BORDER_START_BOTTOM;
	else if (IS_CLAMPED(settings.bottom_border_no_shadow_start, y, settings.bottom_border_no_shadow_end))
		regions |= BORDER_MIDDLE_BOTTOM;
	else if (IS_CLAMPED(settings.bottom_border_no_shadow_end, y, settings.bottom_border_end))
		regions |= BORDER_END_BOTTOM;

	if (IS_CLAMPED(settings.header_border_start, y, settings.header_border_no_shadow_start))
		regions |= BORDER_START_INTERNAL;
	else if (IS_CLAMPED(settings.header_border_no_shadow_start, y, settings.header_border_no_shadow_end))
		regions |= BORDER_MIDDLE_INTERNAL;
	else if (IS_CLAMPED(settings.header_border_no_shadow_end, y, settings.header_border_end))
		regions |= BORDER_END_INTERNAL;

	return regions;
}

static enum border_shadow try_solve_color_conflict(const enum border_region region, const int x, const int y)
{
	const struct border_conflict_solver *solver = NULL;

	for (size_t i = 0; i < sizeof conflict_solvers / sizeof *conflict_solvers; i++)
	{
		if (conflict_solvers[i].region == region)
		{
			solver = conflict_solvers + i;
			break;
		}
	}

	if (solver == NULL)
		return WEIRD_SHADOW;

	if ((int)F(solver->m, solver->p, x) == y)
		return DARK_SHADOW;
	const float xA    = 0.f, yA = F(solver->m, solver->p, xA);
	const float xB    = 1.f, yB = F(solver->m, solver->p, xB);
	const float cross = ((x - xA) * (yB - yA)) - ((y - yA) * (xB - xA));
	if (signbit(cross))
		return solver->right_shadow;
	return solver->left_shadow;
}

t_color select_color(const int x, const int y)
{
	init_settings();

	const enum border_region region = find_border_region(x, y);
	if (region == NO_BORDER)
		return (t_color){0};

	enum border_shadow shadow = get_border_shadow_by_region(region);
	if (shadow == WEIRD_SHADOW)
		shadow = try_solve_color_conflict(region, x, y);

	switch (shadow)
	{
	case WEIRD_SHADOW:
		return settings.weird_shadow;
	case NO_SHADOW:
		break;
	case LIGHT_SHADOW:
		return settings.light_shadow;
	case DARK_SHADOW:
		return settings.dark_shadow;
	}

	return settings.no_shadow;
}
