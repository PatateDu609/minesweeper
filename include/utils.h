#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

typedef union u_color t_color;

int *sort(int *arr, int len);
char *strjoin(const char *, const char *);
int *find_sorted(int *arr, int len, int val);
void set_pixel(SDL_Surface *surface, int x, int y, t_color color);

#endif
