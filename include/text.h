#ifndef MINESWEEPER_TEXT_H
#define MINESWEEPER_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL_rect.h>


extern FT_Library ft_lib;
extern FT_Face ft_win_title_face;

SDL_Surface *render_title(const char *title,  t_color stroke, t_header *hdr);

#endif //MINESWEEPER_TEXT_H
