#include <assert.h>
#include <logger.h>
#include <SDL_image.h>

#include "defines.h"
#include "minesweeper.h"
#include "text.h"
#include "utils.h"

FT_Library ft_lib;
FT_Face    ft_win_title_face;

void init_freetype()
{
	const char *win_title_font = "resources/NotoSans.ttf";

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
	FT_Error error = FT_Set_Char_Size(
		ft_win_title_face,
		0,
		4.35 * 64,
		300,
		300
	);
	if (error)
	{
		console_error("error: FT: couldn't set size font face: %s: %s", ft_win_title_face->family_name, FT_Error_String(error));
		exit(1);
	}

	size_t w, h;
	get_title_size(title, &w, &h);
	FT_Matrix scale = {0x10000L, 0, 0, 0x10000L};

	float scale_ratio = 1.f;

	const float max_size = hdr->rect.h * .75f;
	if (max_size < h)
	{
		const float aspect_ratio = w / (float)h;
		scale_ratio              = max_size / (float)h;

		h = max_size;
		w = h * aspect_ratio;

		scale.xx *= scale_ratio;
		scale.yy *= scale_ratio;
	}


	FT_Vector delta;
	const int x0 = hdr->icon_rect.x + hdr->icon_rect.w + 4;
	const int y0 = labs(hdr->rect.h - (int)h) / 2;
	delta.x      = (FT_Pos)(64 * 0);
	delta.y      = (FT_Pos)(64 * y0);

	SDL_Surface *  txt      = create_rgb_surface(w, h);
	const SDL_Rect txt_rect = txt->clip_rect;

	FT_GlyphSlot slot           = ft_win_title_face->glyph;
	size_t       height         = txt_rect.h;
	size_t       below_baseline = 0;
	for (size_t i = 0; title[i]; i++)
	{
		FT_Set_Transform(ft_win_title_face, NULL, &delta);

		error = FT_Load_Char(ft_win_title_face, title[i], FT_LOAD_RENDER);
		if (error)
		{
			console_error("error: FT: couldn't render current char %c: %s: %s", title[i], ft_win_title_face->family_name, FT_Error_String(error));
			continue;
		}

		render_glyph(hdr->bg, txt, stroke, &slot->bitmap, slot->bitmap_left, h - slot->bitmap_top);

		delta.x += slot->advance.x;
		delta.y += slot->advance.y;
		height = height < slot->bitmap.rows ? slot->bitmap.rows : height;

		const size_t tmp = labs(slot->metrics.horiBearingY - slot->metrics.height) >> 6;
		below_baseline   = below_baseline < tmp ? tmp : below_baseline;
	}

	hdr->title_rect.w = txt_rect.w;
	hdr->title_rect.h = txt_rect.h;
	hdr->title_rect.x = x0;
	hdr->title_rect.y = y0 + below_baseline * scale_ratio;
	return txt;
}
