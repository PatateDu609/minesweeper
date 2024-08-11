#include <assert.h>
#include <logger.h>
#include <SDL_image.h>

#include "minesweeper.h"
#include "text.h"
#include "utils.h"

FT_Library ft_lib;
FT_Face    ft_win_title_face;

void init_freetype()
{
	const char *win_title_font = "resources/tahoma.ttf";

	FT_Error error = FT_Init_FreeType(&ft_lib);
	if (error)
	{
		console_error("error: FT: couldn't init freetype: %s", FT_Error_String(error));
		exit(1);
	}

	FT_Int amajor;
	FT_Int aminor;
	FT_Int apatch;
	FT_Library_Version(ft_lib, &amajor, &aminor, &apatch);

	console_info("Loaded FreeType, %d.%d.%d", amajor, aminor, apatch);


	error = FT_New_Face(ft_lib, win_title_font, 0, &ft_win_title_face);
	if (error == FT_Err_Unknown_File_Format)
	{
		console_error("error: unsupported font format for file: %s", win_title_font);
		exit(1);
	}
	if (error)
	{
		console_error("error: FT: couldn't open font face: %s: %s", win_title_font, FT_Error_String(error));
		exit(1);
	}

	char msg[1024];
	sprintf(msg, "Loaded font %s (%s), nb_glyphs = %zu", ft_win_title_face->family_name, ft_win_title_face->style_name, ft_win_title_face->num_glyphs);
	console_info(msg);
}

void close_freetype()
{
	FT_Done_Face(ft_win_title_face);
	FT_Done_FreeType(ft_lib);
}

void get_title_size(const char *title, size_t *w, size_t *h)
{
	const size_t len = strlen(title);

	const FT_BBox bbox = ft_win_title_face->bbox;

	*w = ((bbox.xMax - bbox.xMin) * len) >> 6;
	*h = (bbox.yMax - bbox.yMin) >> 6;
}


static void render_glyph(SDL_Texture *bg, SDL_Surface *surface, t_color stroke, const FT_Bitmap *bitmap, const FT_Int x, const FT_Int y)
{
	int w, h;
	SDL_QueryTexture(bg, NULL, NULL, &w, &h);

	t_color * bg_pixels;
	int       pitch;
	const int res = SDL_LockTexture(bg, NULL, (void**)&bg_pixels, &pitch);
	assert(res == 0);

	const SDL_Rect bitmap_rect = {
		.x = x,
		.y = y,
		.w = bitmap->width,
		.h = bitmap->rows,
	};

	for (size_t bitmap_y = 0; bitmap_y < bitmap_rect.h; bitmap_y++)
	{
		for (size_t bitmap_x = 0; bitmap_x < bitmap_rect.w; bitmap_x++)
		{
			const uint8_t value = bitmap->buffer[bitmap_y * bitmap->pitch + bitmap_x];
			set_pixel(surface, x + bitmap_x, y + bitmap_y, get_color(stroke.r, stroke.g, stroke.b, stroke.a * value / 255.f));
		}
	}

	SDL_UnlockTexture(bg);
}


SDL_Surface *render_title(const char *title, const t_color stroke, t_header *hdr)
{
	const FT_Error error = FT_Set_Char_Size(
		ft_win_title_face,
		0,
		4.5 * 64,
		300,
		300
	);
	if (error)
	{
		console_error("error: FT: couldn't set size font face: %s: %s", ft_win_title_face->family_name, FT_Error_String(error));
		exit(1);
	}

	const int    x0     = hdr->icon_rect.x + hdr->icon_rect.w + 4;
	struct text *txt    = new_text(title, ft_win_title_face);
	const size_t height = hdr->rect.h;

	txt->stroke = stroke;

	text_compute_bbox(txt);
	const size_t width = txt->bbox.xMax - txt->bbox.xMin;
	text_center(txt, 0, height);
	SDL_Surface *surface = create_rgb_surface(width, height);

	text_render(txt, surface, 0.35);

	hdr->title_rect.w = width;
	hdr->title_rect.h = height;
	hdr->title_rect.x = x0;
	hdr->title_rect.y = (hdr->rect.h - height) / 2;
	return surface;
}
