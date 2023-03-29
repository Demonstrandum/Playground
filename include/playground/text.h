#pragma once

#include "common.h"
#include "mesh.h"

#include <vector>

#include <bgfx/bgfx.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace text {

using mesh::RGBA32;
using Bitmap = FT_Bitmap;

struct Metrics {
    u16 bbox_ymax;
    u16 glyph_width;
    u16 advance;
    u16 xoff;
    u16 yoff;

    Metrics(FT_Face face) {
        bbox_ymax = face->bbox.yMax / 64;
        glyph_width = face->glyph->metrics.width / 64;
        advance = face->glyph->metrics.horiAdvance / 64;
        xoff = (advance - glyph_width) / 2;
        yoff = bbox_ymax - face->glyph->metrics.horiBearingY / 64;
    }
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
    bgfx::TextureHandle renderText(std::vector<rune>);
};

}
