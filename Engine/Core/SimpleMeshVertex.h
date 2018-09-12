#pragma once

#include <bgfx/bgfx.h>

namespace atlas
{
// mesh vertex declaration
struct SimpleMeshVertex
{
    float x;
    float y;
    float z;
    float u;
    float v;
    u32   normal;
    //    u32   tangent;

    static void init()
    {
        vertDecl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            //            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .end();
    }

    static size_t size()
    {
        return sizeof(SimpleMeshVertex);
    }

    static bgfx::VertexDecl vertDecl;
};
}  // atlas namespace
