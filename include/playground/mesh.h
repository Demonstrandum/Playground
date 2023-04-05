#pragma once

#include "common.h"
#include "algebra.h"

#include <vector>
#include <sstream>
#include <bit>

#include <bgfx/bgfx.h>

namespace mesh {

/// RGBA 32-bit 8-bits per channel colour, big endian.
union RGBA32 {
    u32 rgba32;
    u8 rgba[4];

    RGBA32() : rgba32(0x00000000) { }
    RGBA32(u32 val) {
        if constexpr (std::endian::native == std::endian::big) {
            rgba32 = val;
        } else {
            //rgba32 = __builtin_bswap32(val);
            u8* abgr = (u8*)&val;
            rgba32 = 0
                | (u32)abgr[3] << 0
                | (u32)abgr[2] << 8
                | (u32)abgr[1] << 16
                | (u32)abgr[0] << 24;
        }
    }
    RGBA32(u32 r, u8 g, u8 b, u8 a = 0xff)
    { rgba32 = 0
        | (u32)r << 0
        | (u32)g << 8
        | (u32)b << 16
        | (u32)a << 24; }

    u8 red()   const { return (rgba32 >>  0) & 0xff; }
    u8 green() const { return (rgba32 >>  8) & 0xff; }
    u8 blue()  const { return (rgba32 >> 16) & 0xff; }
    u8 alpha() const { return (rgba32 >> 24) & 0xff; }

    static RGBA32 bigEndian(u32 val)
    {
        RGBA32 c;
        c.rgba32 = val;
        return c;
    }
};

std::ostream& operator<<(std::ostream&, const RGBA32&);

#pragma pack(push, 1)
struct Vertex {
    f32 x = 0, y = 0, z = 0;
    f32 u = 0, v = 0;
    RGBA32 color = RGBA32(0xFFFFFFFF);

    Vertex() { }
    Vertex(f32 e1, f32 e2, f32 e3, f32 uv1 = 0, f32 uv2 = 0) : x(e1), y(e2), z(e3), u(uv1), v(uv2) { }
    Vertex(vec3 pos) : x(pos.x), y(pos.y), z(pos.z) { }
    Vertex(vec3 pos, vec2 uv) : x(pos.x), y(pos.y), z(pos.z), u(uv.x), v(uv.y) { }
    vec3 pos() const { return vec3{ x, y, z }; }
    vec2 uv() const { return vec2{ u, v }; }

    static bgfx::VertexLayout layout;
    static u0 init()
    {
        layout.begin()
            .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
        .end();
    }
};
#pragma pack(pop)

 using VertexBuffer = std::vector<Vertex>;

 struct Mesh {
     VertexBuffer vertices;
     std::vector<u16> indices;

     bgfx::Memory const* vertexBuffer()
     {
         const auto* data = vertices.data();
         const usize width = sizeof(*data) * vertices.size();
         return bgfx::makeRef((umin*)data, width);
     }

    bgfx::Memory const* indexBuffer()
     {
         const auto* data = indices.data();
         const usize width = sizeof(*data) * indices.size();
         return bgfx::makeRef((umin*)data, width);
     }
 };

Mesh quad(vec2 pos, f32 w, f32 h);

 } // namespace mesh
