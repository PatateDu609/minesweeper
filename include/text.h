#ifndef MINESWEEPER_TEXT_H
#define MINESWEEPER_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <SDL_surface.h>

#include "color.h"
#include "structs.h"


extern FT_Library ft_lib;
extern FT_Face    ft_win_title_face;


struct text_slot
{
	char      raw;
	FT_UInt   glyph_idx;
	FT_Glyph  glyph;
	FT_Vector pos;
};


struct text
{
	FT_Face face;
	FT_Bool use_kerning;

	size_t            len;
	const char *      raw;
	struct text_slot *slots;

	FT_Vector delta;
	FT_Matrix transform;
	t_color   stroke;

	FT_BBox bbox;
};


SDL_Surface *render_title(const char *title, t_color stroke, t_header *hdr);

struct text *new_text(const char *raw, FT_Face font);
void         text_compute_bbox(struct text *txt);
void         text_render(const struct text *txt, SDL_Surface *target, double boldness);
void         text_center(struct text *txt, size_t width, size_t height);
void         free_text(struct text *txt);

#endif //MINESWEEPER_TEXT_H
