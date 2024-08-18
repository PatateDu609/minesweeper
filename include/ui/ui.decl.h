#pragma once

#include <stdint.h>
#include <stdlib.h>

struct linear_gradient;
struct stop;

struct text;
struct text_slot;

struct btn_group;
struct button;
struct mouse_evt;

typedef struct btn_group  t_btn_group;
typedef struct button *   t_button;
typedef struct mouse_evt *t_mouse_evt;

typedef struct linear_gradient t_linear_gradient;
typedef struct stop            t_stop;

typedef uint8_t ( hover_fn )(struct button *, size_t, size_t);
typedef uint8_t ( click_fn )(struct button *, size_t, size_t);
