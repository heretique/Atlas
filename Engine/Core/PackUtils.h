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

inline u32 packF4u(float x, float y = 0.0f, float z = 0.0f, float w = 0.0f)
{
    const u8 xx = u8(x * 127.0f + 128.0f);
    const u8 yy = u8(y * 127.0f + 128.0f);
    const u8 zz = u8(z * 127.0f + 128.0f);
    const u8 ww = u8(w * 127.0f + 128.0f);
    return packUint32(xx, yy, zz, ww);
}
}  // atlas namespace
