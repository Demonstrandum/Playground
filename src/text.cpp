#include "common.h"
#include "text.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <bgfx/bgfx.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

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

// Call `FT_Bitmap_Done` once finished.
Bitmap FontShaper::renderGlyph(rune character)
{
    loadGlyphBitmap(character);
    ierr err = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    if (err != 0) std::abort();

    Bitmap bitmap = { 0 };
    err = FT_Bitmap_Copy(m_lib, &m_face->glyph->bitmap, &bitmap);
    if (err != 0) std::abort();

    return bitmap;
}

TextTexture FontShaper::renderText(const std::string& s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return renderText(conv.from_bytes(s));
}

TextTexture FontShaper::renderText(const std::u32string& s)
{
    std::vector<rune> runes(s.begin(), s.end());
    return renderText(runes);
}

TextTexture FontShaper::renderText(const std::vector<rune>& text_line)
{
    u16 span = 0;
    u16 height = m_height;
    u16 ascent, descent;
    usize char_count = text_line.size();

    std::vector<Bitmap>  bitmaps;
    std::vector<Metrics> metrics;
    bitmaps.reserve(char_count);
    metrics.reserve(char_count);

    for (usize i = 0; i < text_line.size(); ++i) {
        const Bitmap bm = renderGlyph(text_line[i]);
        const Metrics mt = Metrics(m_face);
        bitmaps.push_back(bm);
        metrics.push_back(mt);
        span += mt.advance;
        if (mt.glyph_height > height)
            height = mt.glyph_height;
    }

    f32 pixelRatio = (f32)m_face->size->metrics.y_ppem / (f32)m_face->units_per_EM;
    ascent  =  m_face->bbox.yMax * pixelRatio;
    descent = -m_face->bbox.yMin * pixelRatio;

    std::vector<RGBA32> bitmap;
    bitmap.resize(span * height, RGBA32(0x00'00'00'00));

    ufast xpos = 0;
    ufast ypos = 0;
    for (usize i = 0; i < bitmaps.size(); ++i) {
        auto bm = bitmaps[i];
        auto mt = metrics[i];

        for (ufast row = 0; row < bm.rows; ++row) {
            ufast row_offset = row + (ascent - descent) - mt.yoff;
            for (ufast col = 0; col < bm.width; ++col) {
                u8 p = bm.buffer[row * bm.pitch + col];
                usize pixel = span * row_offset + (xpos + col + mt.xoff);
                if (p > 0) bitmap[pixel] = RGBA32(0xff, 0xff, 0xff, p);
            }
        }
        xpos += mt.advance;
    }

#if 0
    std::ofstream fout("text.pam");
    fout << "P7\n";
    fout << "WIDTH " << span << "\n";
    fout << "HEIGHT " << height << "\n";
    fout << "DEPTH " << 4 << "\n";
    fout << "MAXVAL " << 255 << "\n";
    fout << "TUPLTYPE " << "RGB_ALPHA" << "\n";
    fout << "ENDHDR\n";
    for (auto& c : bitmap) {
        fout << c.red() << c.green() << c.blue() << c.alpha();
    }
    fout << std::endl;
#endif

    auto handle = bgfx::createTexture2D(span, height, false, 1,
        bgfx::TextureFormat::RGBA8, 0,
        bgfx::copy(bitmap.data(), sizeof(RGBA32) * bitmap.size())
    );

    return (TextTexture){
        .rectWidth = span,
        .rectHeight = height,
        .rectAspect = (f32)span / (f32)height,
        .handle = handle,
    };
}

} // namespace text
