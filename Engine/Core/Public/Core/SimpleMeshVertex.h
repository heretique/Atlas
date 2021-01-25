#pragma once

#include <Hq/BasicTypes.h>
#include <bgfx/c99/bgfx.h>

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
        bgfx_vertex_layout_begin(&vertLayout, bgfx_get_renderer_type());
        bgfx_vertex_layout_add(&vertLayout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_layout_add(&vertLayout, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_layout_add(&vertLayout, BGFX_ATTRIB_NORMAL, 4, BGFX_ATTRIB_TYPE_UINT8, true, true);
        bgfx_vertex_layout_end(&vertLayout);
    }

    static size_t size()
    {
        return sizeof(SimpleMeshVertex);
    }

    static bgfx_vertex_layout_t vertLayout;
};
}  // atlas namespace
