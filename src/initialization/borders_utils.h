#ifndef BORDER_UTILS_H
#define BORDER_UTILS_H

#define BORDER_DARK_ZONE 0.22
#define BORDER_LIGHT_ZONE 0.22

struct borders_settings
{
    int left_border_start;
    int left_border_no_shadow_start;
    int left_border_no_shadow_end;
    int left_border_end;

    int top_border_start;
    int top_border_no_shadow_start;
    int top_border_no_shadow_end;
    int top_border_end;

    int right_border_start;
    int right_border_no_shadow_start;
    int right_border_no_shadow_end;
    int right_border_end;

    int bottom_border_start;
    int bottom_border_no_shadow_start;
    int bottom_border_no_shadow_end;
    int bottom_border_end;

    int header_border_start;
    int header_border_no_shadow_start;
    int header_border_no_shadow_end;
    int header_border_end;

    t_color no_shadow;
    t_color weird_shadow;
    t_color light_shadow;
    t_color dark_shadow;
};


enum border_shadow : uint8_t
{
    WEIRD_SHADOW,
    NO_SHADOW,
    LIGHT_SHADOW,
    DARK_SHADOW
};


enum border_region
{
    NO_BORDER = 0,

    BORDER_START_TOP  = 0b001,
    BORDER_MIDDLE_TOP = 0b010,
    BORDER_END_TOP    = 0b100,

    BORDER_START_LEFT  = 0b001 << 3,
    BORDER_MIDDLE_LEFT = 0b010 << 3,
    BORDER_END_LEFT    = 0b100 << 3,

    BORDER_START_BOTTOM  = 0b001 << 6,
    BORDER_MIDDLE_BOTTOM = 0b010 << 6,
    BORDER_END_BOTTOM    = 0b100 << 6,

    BORDER_START_RIGHT  = 0b001 << 9,
    BORDER_MIDDLE_RIGHT = 0b010 << 9,
    BORDER_END_RIGHT    = 0b100 << 9,

    BORDER_START_INTERNAL  = 0b001 << 12,
    BORDER_MIDDLE_INTERNAL = 0b010 << 12,
    BORDER_END_INTERNAL    = 0b100 << 12,
};

struct border_conflict_solver
{
    enum border_region region;
    float              m, p; // diagonale coeffs
    enum border_shadow left_shadow, right_shadow;
};

#define F(m, p, x) ((m) * (x) + (p))

#endif
