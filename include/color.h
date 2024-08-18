#pragma once

#include <stdint.h>


typedef union u_color
{
	uint32_t raw;


	struct
	{
		uint8_t a;
		uint8_t b;
		uint8_t g;
		uint8_t r;
	};
} t_color;
