#pragma once

#include <cstddef>
#include <string>

namespace atlas
{
class StringHash
{
public:
    // used for unordered_map
    struct Hasher
    {
        size_t operator()(const StringHash& s) const;
    };

    StringHash();
    StringHash(const char* str);
    StringHash(const std::string& str);
    bool operator==(const StringHash& other) const;
    operator size_t() const;

    size_t hash() const;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(_hash);
    }

private:
    size_t _hash;
};

inline size_t StringHash::Hasher::operator()(const StringHash& s) const
{
    // StringHash objects are already hashed during creation, return hash
    return s._hash;
}

inline bool StringHash::operator==(const StringHash& other) const
{
    return _hash == other._hash;
}

inline StringHash::operator size_t() const
{
    return _hash;
}

inline uint32_t rotl32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

inline uint32_t getblock32(const uint32_t* p, int i)
{
    return p[i];
}

inline uint32_t fmix32(uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}
}  // atlas namespace

namespace std
{
template <>
struct hash<atlas::StringHash>
{
    size_t operator()(const atlas::StringHash& s) const
    {
        return s;
    }
};
}
