#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Components/Component.h"
#include "Core/Types.h"
#include <bgfx/bgfx.h>

namespace atlas
{
// mesh vertex declaration
struct MeshVertex
{
    float x;
    float y;
    float z;
    u32   normal;
    u32   tangent;
    u32   color;
    float u;
    float v;

    static void init()
    {
        _vertDecl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexDecl _vertDecl;
};

u32 packUint32(u8 x, u8 y, u8 z, u8 w)
{
    union {
        u32 ui32;
        u8  arr[4];
    } un;

    un.arr[0] = x;
    un.arr[1] = y;
    un.arr[2] = z;
    un.arr[3] = w;

    return un.ui32;
}

u32 packF4u(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
{
    const u8 xx = u8(x * 127.0f + 128.0f);
    const u8 yy = u8(y * 127.0f + 128.0f);
    const u8 zz = u8(z * 127.0f + 128.0f);
    const u8 ww = u8(w * 127.0f + 128.0f);
    return packUint32(xx, yy, zz, ww);
}

struct MeshComponent : public Component
{
public:
    template <class Archive>
    void serialize(Archive& ar)
    {
    }
};

}  // namespace Atlas

#endif  // MESHCOMPONENT_H
