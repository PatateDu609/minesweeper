#include "minesweeper.h"

char *strjoin(const char *s1, const char *s2)
{
	size_t len1 = strlen(s1), len2 = strlen(s2);
	char *result = calloc(len1 + len2 + 1, sizeof(char));
	if (!result)
		return NULL;
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2);
	return result;
}

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

static int partition(int *arr, int s, int e)
{
	int pivot = arr[e];
	int i = s - 1;

	for (int j = s; j < e; j++)
	{
		if (arr[j] < pivot)
			swap(arr + j, arr + ++i);
	}
	swap(arr + i + 1, arr + e);
	return i + 1;
}

static void __sort(int *arr, int s, int e)
{
	if (s < e)
	{
		int pivot = partition(arr, s, e);
		__sort(arr, s, pivot - 1);
		__sort(arr, pivot + 1, e);
	}
}

int *sort(int *arr, int len)
{
	__sort(arr, 0, len - 1);
	return arr;
}

static int *__find_sorted(int *arr, int s, int e, int val)
{
	if (s > e)
		return NULL;
	int m = s + (e - s) / 2;

	if (arr[m] == val)
		return arr + m;

	int *result = __find_sorted(arr, s, m - 1, val);
	if (result && *result == val)
		return result;

	return __find_sorted(arr, m + 1, e, val);
}

int *find_sorted(int *arr, int len, int val)
{
	return __find_sorted(arr, 0, len - 1, val);
}

void set_pixel(SDL_Surface *surface, int x, int y, t_color color)
{
	Uint32 *target_pixel = (Uint32 *)((Uint8 *)surface->pixels +
									  y * surface->pitch +
									  x * surface->format->BytesPerPixel);
	*target_pixel = color.raw;
}
