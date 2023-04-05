#pragma once

#include "common.h"
#include "mesh.h"

#include <vector>
#include <codecvt>


#include <bgfx/bgfx.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace text {

using mesh::RGBA32;
using Bitmap = FT_Bitmap;

struct Metrics {
    u16 glyph_width;
    u16 glyph_height;
    u16 advance;
    u16 xoff;
    u16 yoff;

    Metrics(FT_Face face) {
        glyph_width = face->glyph->metrics.width / 64;
        glyph_height = face->glyph->metrics.height / 64;
        advance = face->glyph->metrics.horiAdvance / 64;
        xoff = (advance - glyph_width) / 2;
        yoff = face->glyph->bitmap_top;
    }
};

/// Blitted texture of text on the same line.
struct TextTexture {
    u16 rectWidth;
    u16 rectHeight;
    f32 rectAspect;
    bgfx::TextureHandle handle;
};

struct FontShaper {
    FT_Library m_lib = nullptr;
    FT_Face m_face;
    u16 m_height;

    FontShaper();
    ~FontShaper();

    ierr loadFontFace(std::vector<umin>, u16);
    ierr setPixelHeight(u16);
    usize glyphCharacterIndex(rune);
    Metrics loadGlyphMetrics(rune);
    ierr loadGlyphBitmap(rune);
    Bitmap renderGlyph(rune);
    TextTexture renderText(const std::vector<rune>&);
    TextTexture renderText(const std::u32string&);
    TextTexture renderText(const std::string&);
};

}
