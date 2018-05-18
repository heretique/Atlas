#pragma once

#include <stdint.h>
#include <stdio.h>

namespace atlas
{
namespace murmur
{
    template <int...>
    struct Sequence
    {
    };
    template <int N, int... S>
    struct CreateSequence : CreateSequence<N - 1, N - 1, S...>
    {
    };
    template <int... S>
    struct CreateSequence<0, S...>
    {
        using Type = Sequence<S...>;
    };

    constexpr uint32_t UpdateHash1(uint32_t k)
    {
        return k * 0xcc9e2d51;
    }
    constexpr uint32_t UpdateHash2(uint32_t k)
    {
        return (k << 15) | (k >> (32 - 15));
    }
    constexpr uint32_t UpdateHash3(uint32_t k)
    {
        return k * 0x1b873593;
    }
    constexpr uint32_t UpdateHash4(uint32_t hash, uint32_t block)
    {
        return hash ^ block;
    }
    constexpr uint32_t UpdateHash5(uint32_t hash)
    {
        return ((hash << 13) | (hash >> (32 - 13))) * 5 + 0xe6546b64;
    }

    constexpr uint32_t UpdateHash(uint32_t hash, uint32_t block)
    {
        return UpdateHash5(UpdateHash4(hash, UpdateHash3(UpdateHash2(UpdateHash1(block)))));
    }

    constexpr uint32_t UpdateLastHash(uint32_t hash, uint32_t block)
    {
        return UpdateHash4(hash, UpdateHash3(UpdateHash2(UpdateHash1(block))));
    }

    template <typename... C>
    constexpr uint32_t CalculateHashRounds(uint32_t seed, C... c);

    template <>
    constexpr uint32_t CalculateHashRounds(uint32_t seed)
    {
        return seed;
    }

    template <>
    constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0)
    {
        return UpdateLastHash(seed, uint8_t(c0));
    }

    template <>
    constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1)
    {
        return UpdateLastHash(seed, uint8_t(c0) | uint8_t(c1) << 8);
    }

    template <>
    constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1, char c2)
    {
        return UpdateLastHash(seed, uint8_t(c0) | uint8_t(c1) << 8 | uint8_t(c2) << 16);
    }

    template <typename... C>
    constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1, char c2, char c3, C... c)
    {
        return CalculateHashRounds(
            UpdateHash(seed, uint8_t(c0) | uint8_t(c1) << 8 | uint8_t(c2) << 16 | uint8_t(c3) << 24), c...);
    }

    constexpr uint32_t CalculateFinalHash1(uint32_t h, uint32_t length)
    {
        return h ^ length;
    }

    constexpr uint32_t CalculateFinalHash2(uint32_t h)
    {
        return h ^ (h >> 16);
    }

    constexpr uint32_t CalculateFinalHash3(uint32_t h)
    {
        return h * 0x85ebca6b;
    }

    constexpr uint32_t CalculateFinalHash4(uint32_t h)
    {
        return h ^ (h >> 13);
    }

    constexpr uint32_t CalculateFinalHash5(uint32_t h)
    {
        return h * 0xc2b2ae35;
    }

    constexpr uint32_t CalculateFinalHash6(uint32_t h)
    {
        return h ^ (h >> 16);
    }

    constexpr uint32_t CalculateFinalHash(uint32_t h, uint32_t length)
    {
        return CalculateFinalHash6(CalculateFinalHash5(
            CalculateFinalHash4(CalculateFinalHash3(CalculateFinalHash2(CalculateFinalHash1(h, length))))));
    }

    // This is used to convert from calling const char (&s)[N]
    // To CalculateHashRounds(seed, s[0], s[1], s[2], s[3], ... )
    template <int N, int... S>
    constexpr uint32_t Unpack(unsigned seed, const char (&s)[N], Sequence<S...>)
    {
        return CalculateHashRounds(seed, s[S]...);
    }
}  // murmur namespace

template <int N>
constexpr uint32_t MURMUR3_32(const char (&s)[N], uint32_t seed = 0)
{
    return murmur::CalculateFinalHash(murmur::Unpack(seed, s, typename murmur::CreateSequence<N - 1>::Type()), N - 1);
}

}  // atlas namespace
