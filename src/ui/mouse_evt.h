#ifndef UI_MOUSE_EVT_H
#define UI_MOUSE_EVT_H

#include "ui/button.h"


enum mouse_evt_type
{
	CLICK,
	MOTION
};


struct mouse_evt
{
	enum mouse_evt_type type;
	uint32_t            timestamp;

	uint32_t windowID;
	uint32_t which;
	int32_t  x;
	int32_t  y;
};


struct mouse_click_evt
{
	struct mouse_evt evt;

	uint8_t button;
	uint8_t clicks;
	uint8_t state;
};


struct mouse_motion_evt
{
	struct mouse_evt evt;

	uint32_t state;
	int32_t  xrel;
	int32_t  yrel;
};

#endif //UI_MOUSE_EVT_H
