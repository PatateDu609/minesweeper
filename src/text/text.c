#include <assert.h>
#include <logger.h>

#include "text.h"

#include "utils.h"

static void free_txt_slots(struct text *txt)
{
	if (txt->slots == NULL)
		return;

	for (size_t i = 0; i < txt->len; i++)
		FT_Done_Glyph(txt->slots[i].glyph);

	free(txt->slots);
	txt->slots = NULL;
}

void text_compute_bbox(struct text *txt)
{
	FT_BBox *bbox = &txt->bbox;
	FT_BBox  glyph_bbox;

	bbox->xMin = bbox->yMin = 32000;
	bbox->xMax = bbox->yMax = -32000;

	for (size_t i = 0; i < txt->len; i++)
	{
		const struct text_slot *slot = txt->slots + i;
		FT_Glyph_Get_CBox(slot->glyph, ft_glyph_bbox_pixels, &glyph_bbox);

		glyph_bbox.xMin += slot->pos.x;
		glyph_bbox.xMax += slot->pos.x;
		glyph_bbox.yMin += slot->pos.y;
		glyph_bbox.yMax += slot->pos.y;

		if (glyph_bbox.xMin < bbox->xMin)
			bbox->xMin = glyph_bbox.xMin;

		if (glyph_bbox.yMin < bbox->yMin)
			bbox->yMin = glyph_bbox.yMin;

		if (glyph_bbox.xMax > bbox->xMax)
			bbox->xMax = glyph_bbox.xMax;

		if (glyph_bbox.yMax > bbox->yMax)
			bbox->yMax = glyph_bbox.yMax;
	}

	if (bbox->xMin > bbox->xMax)
	{
		bbox->xMin = 0;
		bbox->yMin = 0;
		bbox->xMax = 0;
		bbox->yMax = 0;
	}
}

uint8_t text_init_slots(struct text *txt)
{
	static char msg[512];

	free_txt_slots(txt);
	txt->slots = calloc(txt->len, sizeof *txt->slots);
	assert(txt->slots != NULL);

	const FT_GlyphSlot slot = txt->face->glyph;

	int       pen_x    = 0;
	FT_UInt   previous = 0;

	uint8_t res = 1;
	for (size_t i = 0; i < txt->len; i++)
	{
		struct text_slot *txt_slot = txt->slots + i;
		txt_slot->raw              = txt->raw[i];

		txt_slot->glyph_idx = FT_Get_Char_Index(txt->face, txt->raw[i]);
		FT_Error error;

		if (txt->use_kerning && previous && txt_slot->glyph_idx)
		{
			FT_Vector delta;
			error = FT_Get_Kerning(txt->face, previous, txt_slot->glyph_idx, FT_KERNING_DEFAULT, &delta);
			if (error)
			{
				sprintf(msg, "error: couldn't get kerning: %s: %s", txt->face->family_name, FT_Error_String(error));
				console_error(msg);
				res = 0;
				continue;
			}
			pen_x += delta.x >> 6;
		}

		txt_slot->pos.x = pen_x;
		txt_slot->pos.y = 0;

		error = FT_Load_Glyph(txt->face, txt_slot->glyph_idx, FT_LOAD_DEFAULT);
		if (error)
		{
			sprintf(msg, "error: couldn't load glyph for char %c: %s: %s", txt->raw[i], txt->face->family_name, FT_Error_String(error));
			console_error(msg);
			res = 0;
			continue;
		}

		error = FT_Get_Glyph(slot, &txt->slots[i].glyph);
		if (error)
		{
			sprintf(msg, "error: couldn't copy glyph: %s: %s", txt->face->family_name, FT_Error_String(error));
			console_error(msg);
			res = 0;
			continue;
		}

		pen_x += slot->advance.x >> 6;
		previous = txt_slot->glyph_idx;
	}

	return res;
}

struct text *new_text(const char *raw, const FT_Face font)
{
	struct text *txt = calloc(1, sizeof *txt);
	assert(txt != NULL);

	txt->raw         = raw;
	txt->len         = strlen(raw);
	txt->face        = font;
	txt->use_kerning = FT_HAS_KERNING(txt->face);

	txt->delta.x      = 0;
	txt->delta.y      = 0;
	txt->transform.xx = 0x10000L;
	txt->transform.xy = 0;
	txt->transform.yx = 0;
	txt->transform.yy = 0x10000L;

	txt->stroke = get_color(255, 255, 255, 255);

	if (!text_init_slots(txt))
	{
		free_text(txt);
		return NULL;
	}

	return txt;
}

void free_text(struct text *txt)
{
	if (txt == NULL)
		return;

	free_txt_slots(txt);
	free(txt);
}
