#include <assert.h>

#include "minesweeper.h"
#include <logger.h>

char *strjoin(const char *s1, const char *s2)
{
	size_t len1   = strlen(s1), len2 = strlen(s2);
	char * result = calloc(len1 + len2 + 1, sizeof(char));
	if (!result)
		return NULL;
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2);
	return result;
}

void swap(uint32_t *a, uint32_t *b)
{
	int t = *a;
	*a    = *b;
	*b    = t;
}

static int partition(uint32_t *arr, int s, int e)
{
	uint32_t pivot = arr[e];
	int      i     = s - 1;

	for (int j = s; j < e; j++)
	{
		if (arr[j] < pivot)
			swap(arr + j, arr + ++i);
	}
	swap(arr + i + 1, arr + e);
	return i + 1;
}

static void __sort(uint32_t *arr, int s, int e)
{
	if (s < e)
	{
		int pivot = partition(arr, s, e);
		__sort(arr, s, pivot - 1);
		__sort(arr, pivot + 1, e);
	}
}

uint32_t *sort(uint32_t *arr, int len)
{
	__sort(arr, 0, len - 1);
	return arr;
}

static uint32_t *__find_sorted(uint32_t *arr, int s, int e, uint32_t val)
{
	if (s > e)
		return NULL;
	int m = s + (e - s) / 2;

	if (arr[m] == val)
		return arr + m;

	uint32_t *result = __find_sorted(arr, s, m - 1, val);
	if (result && *result == val)
		return result;

	return __find_sorted(arr, m + 1, e, val);
}

uint32_t *find_sorted(uint32_t *arr, int len, uint32_t val)
{
	return __find_sorted(arr, 0, len - 1, val);
}

void set_pixel(SDL_Surface *surface, int x, int y, t_color color)
{
	const SDL_Point pt = {x, y};
	if (!SDL_PointInRect(&pt, &surface->clip_rect))
		return;

	Uint32 *target_pixel = (Uint32*)((Uint8*)surface->pixels +
		y * surface->pitch +
		x * surface->format->BytesPerPixel);
	*target_pixel = color.raw;
}

t_color over_op(const t_color bg, const t_color fg)
{
	const static double gamma     = 2.2;
	const static double inv_gamma = 1. / gamma;

	const double fg_alpha = fg.a / 255.;
	const double bg_alpha = bg.a / 255.;
	const double bg_r     = pow(bg.r / 255., gamma), bg_g = pow(bg.g / 255., gamma), bg_b = pow(bg.b / 255., gamma);
	const double fg_r     = pow(fg.r / 255., gamma), fg_g = pow(fg.g / 255., gamma), fg_b = pow(fg.b / 255., gamma);

	const double _1_minus_fg_alpha = 1 - fg_alpha;


	const double r = fg_r + bg_r * _1_minus_fg_alpha;
	const double g = fg_g + bg_g * _1_minus_fg_alpha;
	const double b = fg_b + bg_b * _1_minus_fg_alpha;
	const double a = fg_alpha + bg_alpha * _1_minus_fg_alpha;

	t_color res;
	res.r = (uint8_t)(SDL_clamp(pow(r, inv_gamma), 0, 1) * 255);
	res.g = (uint8_t)(SDL_clamp(pow(g, inv_gamma), 0, 1) * 255);
	res.b = (uint8_t)(SDL_clamp(pow(b, inv_gamma), 0, 1) * 255);
	res.a = (uint8_t)(SDL_clamp(a, 0, 1) * 255);
	return res;
}

SDL_Surface *create_rgb_surface(const size_t width, const size_t height)
{
	static const Uint32 rmask = 0xff000000;
	static const Uint32 gmask = 0x00ff0000;
	static const Uint32 bmask = 0x0000ff00;
	static const Uint32 amask = 0x000000ff;

	return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

t_score_numbers *itoscores(int digits, uint32_t nb)
{
	t_score_numbers *numbers = calloc(digits, sizeof(t_score_numbers));
	if (!numbers)
	{
		console_error("Allocation failed");
		exit(1);
	}

	char            buffer[32];
	t_score_numbers n[10];
	SDL_itoa(nb, buffer, 10);

	n[0] = SM_0;
	n[1] = SM_1;
	n[2] = SM_2;
	n[3] = SM_3;
	n[4] = SM_4;
	n[5] = SM_5;
	n[6] = SM_6;
	n[7] = SM_7;
	n[8] = SM_8;
	n[9] = SM_9;

	if (strlen(buffer) > (size_t)digits)
		for (int i     = 0; i < digits; i++)
			numbers[i] = SM_9;
	else
		for (size_t i  = digits - strlen(buffer), j = 0; j < strlen(buffer); j++, i++)
			numbers[i] = n[buffer[j] - '0'];

	return numbers;
}

t_color get_color(int r, int g, int b, int a)
{
	t_color color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}

SDL_Color get_sdl_color(t_color c)
{
	SDL_Color ret;

	ret.r = c.r;
	ret.g = c.g;
	ret.b = c.b;
	ret.a = c.a;
	return ret;
}

void draw_border(SDL_Surface *target, const uint8_t thickness, const t_color color)
{
	if (thickness == 0)
		return;

	const SDL_Rect rect = target->clip_rect;

	for (uint8_t j = 0; j < thickness; j++)
	{
		for (size_t i = 0; i < rect.w; i++)
		{
			set_pixel(target, i, j, color);
			set_pixel(target, i, rect.h - j - 1, color);
		}

		for (size_t i = 0; i < rect.h; i++)
		{
			set_pixel(target, j, i, color);
			set_pixel(target, rect.w - j - 1, i, color);
		}
	}
}
