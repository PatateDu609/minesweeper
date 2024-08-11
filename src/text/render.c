#include <assert.h>
#include <logger.h>

#include "text.h"
#include "utils.h"

#include FT_BITMAP_H

static void draw_bitmap(SDL_Surface *target, const t_color *stroke, const FT_Bitmap bitmap, const FT_Int left, const FT_Int top)
{
	for (size_t bitmap_y = 0; bitmap_y < bitmap.rows; bitmap_y++)
	{
		for (size_t bitmap_x = 0; bitmap_x < bitmap.width; bitmap_x++)
		{
			const uint8_t value = bitmap.buffer[bitmap_y * bitmap.pitch + bitmap_x];
			set_pixel(target, left + bitmap_x, top + bitmap_y, get_color(stroke->r, stroke->g, stroke->b, stroke->a * value / 255.f));
		}
	}
}

void text_render(const struct text *txt, SDL_Surface *target, const double boldness)
{
	static char msg[512];

	const SDL_Rect  clip = target->clip_rect;
	FT_BBox         bbox;
	FT_Glyph        image;
	FT_Vector       pen    = txt->delta;
	const FT_Matrix matrix = txt->transform;

	for (size_t i = 0; i < txt->len; i++)
	{
		FT_Error error = FT_Glyph_Copy(txt->slots[i].glyph, &image);
		if (error)
		{
			sprintf(msg, "error: couldn't copy glyph: %s: %s", txt->face->family_name, FT_Error_String(error));
			console_error(msg);
			continue;
		}

		FT_Glyph_Transform(image, &matrix, &pen);

		FT_Glyph_Get_CBox(image, ft_glyph_bbox_pixels, &bbox);
		if (bbox.xMax <= clip.x || bbox.xMin >= clip.w ||
			bbox.yMax <= clip.y || bbox.yMin >= clip.h)
			continue;

		error = FT_Glyph_To_Bitmap(
			&image,
			FT_RENDER_MODE_NORMAL,
			NULL,
			1
		);
		if (!error)
		{
			const FT_BitmapGlyph bit      = (FT_BitmapGlyph)image;
			FT_Bitmap            bitmap   = bit->bitmap;
			uint8_t              is_duped = 0;

			if (boldness > 0 + DBL_EPSILON)
			{
				is_duped = 1;
				FT_Bitmap_Init(&bitmap);

				error = FT_Bitmap_Copy(ft_lib, &bit->bitmap, &bitmap);
				if (error)
				{
					sprintf(msg, "error: couldn't copy bitmap: %s: %s", txt->face->family_name, FT_Error_String(error));
					console_error(msg);
					FT_Bitmap_Done(ft_lib, &bitmap);
					bitmap = bit->bitmap;
				}

				error = FT_Bitmap_Embolden(ft_lib, &bitmap, boldness * 64, 0);
				if (error)
				{
					sprintf(msg, "error: couldn't embolden bitmap: %s: %s", txt->face->family_name, FT_Error_String(error));
					console_error(msg);
				}
			}

			draw_bitmap(target, &txt->stroke, bitmap, bit->left, clip.h - bit->top);

			pen.x += image->advance.x >> 10;
			pen.y += image->advance.y >> 10;

			if (is_duped)
				FT_Bitmap_Done(ft_lib, &bitmap);
			FT_Done_Glyph(image);
		}
	}
}

void text_center(struct text *txt, const size_t width, const size_t height)
{
	if (!width && !height)
		return;

	text_compute_bbox(txt);

	/* compute string dimensions in integer pixels */
	const size_t string_width  = txt->bbox.xMax - txt->bbox.xMin;
	const size_t string_height = txt->bbox.yMax - txt->bbox.yMin;

	if (width)
		txt->delta.x = 64 * ((width - string_width) / 2.);

	if (height)
		txt->delta.y = 64 * ((height - string_height) / 2.);
}
