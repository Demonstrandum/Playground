#include "common.h"
#include "text.h"

#include <iostream>
#include <vector>

#include <bgfx/bgfx.h>
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library ft_lib;
static bool ft_lib_ready = false;
static usize ft_lib_ref_count = 0;

namespace text {

FontShaper::FontShaper()
{
	if (!ft_lib_ready) {
		ierr err = FT_Init_FreeType(&ft_lib);
		if (err != 0) return;
		ft_lib_ready = true;
	}
	m_lib = ft_lib;
	ft_lib_ref_count += 1;
}

FontShaper::~FontShaper()
{
	if (ft_lib_ready) {
		ft_lib_ref_count -= 1;
		if (ft_lib_ref_count == 0) FT_Done_FreeType(ft_lib);
	}
}

ierr FontShaper::loadFontFace(std::vector<umin> buf, u16 height = 0)
{
	ierr err = FT_New_Memory_Face(m_lib, buf.data(), buf.size(), 0, &m_face);
	if (height > 0) setPixelHeight(height);
	return err;
}

ierr FontShaper::setPixelHeight(u16 height)
{
	m_height = height;
	return FT_Set_Pixel_Sizes(m_face, 0, m_height);
}

usize FontShaper::glyphCharacterIndex(rune character)
{
	return FT_Get_Char_Index(m_face, (FT_ULong)character);
}

ierr FontShaper::loadGlyphBitmap(rune character)
{
	usize gi = glyphCharacterIndex(character);
	return FT_Load_Glyph(m_face, gi, FT_LOAD_DEFAULT);
}

Metrics FontShaper::loadGlyphMetrics(rune character)
{
	usize gi = glyphCharacterIndex(character);
	FT_Load_Glyph(m_face, gi, FT_LOAD_NO_BITMAP);
	return Metrics(m_face);
}

Bitmap FontShaper::renderGlyph(rune character)
{
	loadGlyphBitmap(character);
	ierr err = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
	if (err != 0) std::abort();
	return m_face->glyph->bitmap;
}

bgfx::TextureHandle FontShaper::renderText(std::vector<rune> text_line)
{
	u16 span = 0;
	u16 height = m_height;
	usize char_count = text_line.size();

	std::vector<Bitmap>  bitmaps;
	std::vector<Metrics> metrics;
	bitmaps.reserve(char_count);
	metrics.reserve(char_count);

	for (usize i = 0; i < text_line.size(); ++i) {
		bitmaps[i] = renderGlyph(text_line[i]);
		metrics[i] = Metrics(m_face);
		span += metrics[i].advance;
	}


	std::vector<RGBA32> bitmap;
	bitmap.resize(4 * span * height, RGBA32(0, 0, 0, 0));

	ufast xpos = 0;
	ufast ypos = 0;
	for (usize i = 0; i < bitmaps.size(); ++i) {
		auto bm = bitmaps[i];
		auto mt = metrics[i];
		for (ufast row = 0; row < bm.rows; ++row) {
			ufast row_offset = ypos + row + mt.yoff;
			for (ufast col = 0; col < bm.width; ++col) {
				u8 p = bm.buffer[row * bm.pitch + col];
				usize pixel = span * (xpos + col + mt.xoff) + row_offset;
				if (p > 0) bitmap[pixel] = RGBA32(p, p, p, 0xff);
			}
		}
		xpos += mt.advance;
	}

	return bgfx::createTexture2D(span, height, false, 1,
		bgfx::TextureFormat::BGRA8, 0,
		bgfx::copy(bitmap.data(), bitmap.size())
	);
}

} // namespace text
