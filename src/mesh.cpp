#include "mesh.h"

namespace mesh {

std::ostream& operator<<(std::ostream& os, const RGBA32& c) {
    return (
        os << std::hex
        << "(r:0x" << (int)c.red()
        << ",g:0x" << (int)c.green()
        << ",b:0x" << (int)c.blue()
        << ",a:0x" << (int)c.alpha()
        << ")"
        << std::dec
    );
}

bgfx::VertexLayout Vertex::layout;

Mesh quad(vec2 pos, f32 w, f32 h)
{
    return Mesh {
        .vertices = {
            //     x          y          z    --  u     v
            Vertex(pos.x,     pos.y,     1.0f,    0.0f, 1.0f),
            Vertex(pos.x + w, pos.y,     1.0f,    1.0f, 1.0f),
            Vertex(pos.x + w, pos.y + h, 1.0f,    1.0f, 0.0f),
            Vertex(pos.x,     pos.y + h, 1.0f,    0.0f, 0.0f)
        },
        .indices = {
            0, 1, 2,  // upper triangle
            2, 3, 0,  // lower triangle
        }
    };
}

}  // namespace mesh
