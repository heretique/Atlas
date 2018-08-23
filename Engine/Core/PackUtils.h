#pragma once

namespace atlas
{
inline u32 packUint32(u8 x, u8 y, u8 z, u8 w)
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

/// pack 4 normalized floats [-1, 1] to u32
inline u32 packNF4u(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
{
    const u8 xx = u8(x * 127.0f + 128.0f);
    const u8 yy = u8(y * 127.0f + 128.0f);
    const u8 zz = u8(z * 127.0f + 128.0f);
    const u8 ww = u8(w * 127.0f + 128.0f);
    return packUint32(xx, yy, zz, ww);
}

/// pack 4 unsigned floats [0, 1] to u32
inline u32 packUF4u(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
{
    const u8 xx = u8(x * 255.f);
    const u8 yy = u8(y * 255.f);
    const u8 zz = u8(z * 255.f);
    const u8 ww = u8(w * 255.f);
    return packUint32(xx, yy, zz, ww);
}

}  // atlas namespace
