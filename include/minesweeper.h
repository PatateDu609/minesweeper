#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "utils.h"
#include "structs.h"

void init(void);
void init_sprites(void);
void init_state(t_game *game);
void free_all(void);

void reset_game(t_game *game);
void start_game(t_game *game, t_coord *coord);
void init_field(void);
void launch(void);

t_coord *get_coord(int wx, int wy);
void mark_tile(t_coord *coord);
void select_tile(t_coord *coord);
int is_in_field(int x, int y);
void flip(t_coord *coord);
void highlight_selected(void);
void set_selected(t_coord *coord);

void mouse_click_down(SDL_MouseButtonEvent button);
void mouse_click_up(SDL_MouseButtonEvent button);
void mouse_move(SDL_MouseMotionEvent motion);

void draw(void);

t_color select_color(int x, int y);

extern t_minesweeper minesweeper;

#endif
