#include "minesweeper.h"
#include <logger.h>

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

void swap(uint32_t *a, uint32_t *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

static int partition(uint32_t *arr, int s, int e)
{
	uint32_t pivot = arr[e];
	int i = s - 1;

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
	Uint32 *target_pixel = (Uint32 *)((Uint8 *)surface->pixels +
									  y * surface->pitch +
									  x * surface->format->BytesPerPixel);
	*target_pixel = color.raw;
}

t_score_numbers *itoscores(int digits, uint32_t nb)
{
	t_score_numbers *numbers = calloc(digits, sizeof(t_score_numbers));
	if (!numbers)
	{
		console_error("Allocation failed");
		exit(1);
	}

	char buffer[32];
	SDL_itoa(nb, buffer, 10);

	if (strlen(buffer) > (size_t)digits)
		for (int i = 0; i < digits; i++)
			numbers[i] = SM_9;
	else
	{
		for (size_t i = digits - strlen(buffer), j = 0; j < strlen(buffer); j++, i++)
		{
			switch (buffer[j])
			{
			case '0':
				numbers[i] = SM_0;
				break;
			case '1':
				numbers[i] = SM_1;
				break;
			case '2':
				numbers[i] = SM_2;
				break;
			case '3':
				numbers[i] = SM_3;
				break;
			case '4':
				numbers[i] = SM_4;
				break;
			case '5':
				numbers[i] = SM_5;
				break;
			case '6':
				numbers[i] = SM_6;
				break;
			case '7':
				numbers[i] = SM_7;
				break;
			case '8':
				numbers[i] = SM_8;
				break;
			case '9':
				numbers[i] = SM_9;
				break;
			}
		}
	}

	return numbers;
}
