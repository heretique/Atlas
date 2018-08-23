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
    //    u32   normal;
    //    u32   tangent;
    u32 color;
    //    float u;
    //    float v;

    static void init()
    {
        vertDecl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            //            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            //            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            //            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    static size_t size()
    {
        return sizeof(SimpleMeshVertex);
    }

    static bgfx::VertexDecl vertDecl;
};
}  // atlas namespace
