#include "StringHash.h"
#include "Types.h"

#include <cstring>

namespace atlas
{
uint32_t MurmurHash3_32(const void* key, uint len, uint32_t seed)
{
    const uint8_t* data    = (const uint8_t*)key;
    const int      nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t* blocks = (const uint32_t*)(data + nblocks * 4);

    for (int i = -nblocks; i; i++)
    {
        uint32_t k1 = getblock32(blocks, i);

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    //----------
    // tail

    const uint8_t* tail = (const uint8_t*)(data + nblocks * 4);

    uint32_t k1 = 0;

    switch (len & 3)
    {
        case 3:
            k1 ^= (unsigned)tail[2] << 16;
        case 2:
            k1 ^= (unsigned)tail[1] << 8;
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    return fmix32(h1);
}

StringHash::StringHash()
    : _hash(0)
{
}

StringHash::StringHash(const char* str)
    : _hash(MurmurHash3_32(str, static_cast<uint>(strlen(str)), 0x12345678))
{
}

StringHash::StringHash(const std::string& str)
    : _hash(MurmurHash3_32(str.c_str(), static_cast<uint>(str.length()), 0x12345678))
{
}

}  // atlas namespace
