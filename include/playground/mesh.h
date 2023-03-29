#pragma once

#include "common.h"
#include "math.h"

#include <vector>

namespace mesh {

/// RGBA 32-bit 8-bits per channel colour w.r.t. endianess.
union RGBA32 {
	u32 rgba32;
	u8 rgba[4];

	RGBA32() : rgba32(0x00000000) { }
	RGBA32(u32 val) : rgba32(val) { }
	RGBA32(u8 r, u8 g, u8 b, u8 a = 0xFF)
	{ rgba32 = (r << 24) | (g << 16) | (b << 8) | a; }
};

struct Vertex {
	f32 x = 0, y = 0, z = 0;
	RGBA32 color = RGBA32(0x00000000);

	Vertex() { }
	Vertex(f32 e1, f32 e2, f32 e3) : x(e1), y(e2), z(e3) { }
	Vertex(vec3 pos) : x(pos.x), y(pos.y), z(pos.z) { }
	vec3 pos() { return vec3{ x, y, z }; }
};

 using VertexBuffer = std::vector<Vertex>;

 } // namespace mesh
