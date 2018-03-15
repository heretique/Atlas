#pragma once

#include "Core/Debug.h"
#include "Core/Types.h"

namespace atlas
{
#pragma pack(push, 1)
template <size_t StorageBits, size_t GenerationBits>
struct Handle
{
    static const Handle<StorageBits, GenerationBits> invalid;

    Handle()
        : _index(0)
        , _generation(0)
    {
    }

    Handle(const Handle<StorageBits, GenerationBits>& other)
        : _index(other._index)
        , _generation(other._generation)
    {
    }

    Handle(uint32_t index, uint32_t generation)
        : _index(index)
        , _generation(generation)
    {
        assert(index <= MaxIndex);
    }

    uint32_t index() const;
    uint32_t generation() const;
    void setIndex(uint32_t index);
    void setGeneration(uint32_t generation);
    bool operator!=(const Handle& other) const;
    bool operator==(const Handle& other) const;
    bool operator<(const Handle& other) const;
    bool valid() const;

private:
    uint32_t _index : StorageBits;
    uint32_t _generation : GenerationBits;

    static const uint32_t MaxIndex = (1 << StorageBits) - 1;
};

template <size_t I, size_t G>
const Handle<I, G>         Handle<I, G>::invalid;

template <size_t I, size_t G>
inline uint32_t Handle<I, G>::index() const
{
    return _index;
}

template <size_t I, size_t G>
inline uint32_t Handle<I, G>::generation() const
{
    return _generation;
}

template <size_t I, size_t G>
inline void Handle<I, G>::setIndex(uint32_t index)
{
    assert(index <= MaxIndex);
    _index = index;
}

template <size_t I, size_t G>
inline void Handle<I, G>::setGeneration(uint32_t generation)
{
    // generation can wrap-around we're not worried about this
    _generation = generation;
}

template <size_t I, size_t G>
inline bool Handle<I, G>::operator!=(const Handle& other) const
{
    return _index != other._index || _generation != other._generation;
}

template <size_t I, size_t G>
inline bool Handle<I, G>::operator==(const Handle& other) const
{
    return _index == other._index && _generation == other._generation;
}
template <size_t I, size_t G>
inline bool Handle<I, G>::valid() const
{
    return *this != Handle<I, G>::invalid;
}

template <size_t I, size_t G>
inline bool Handle<I, G>::operator<(const Handle& other) const
{
    const bool maybe = _index != other._index;

    if (maybe)
        return _index < other._index;

    return _generation < other._generation;
}

template <size_t StorageBits, size_t GenerationBits, size_t TypeBits>
struct TypedHandle
{
    TypedHandle()
        : _index(0)
        , _generation(0)
        , m_type(0)
    {
    }

    TypedHandle(uint32_t index, uint32_t generation, uint32_t type)
        : _index(index)
        , _generation(generation)
        , m_type(type)
    {
        assert(index <= MaxIndex);
        assert(type <= MaxType);
    }

    uint32_t index() const;
    uint32_t generation() const;
    uint32_t type() const;
    void setIndex(uint32_t index);
    void setGeneration(uint32_t generation);
    void setType(uint32_t type);

private:
    uint32_t _index : StorageBits;
    uint32_t _generation : GenerationBits;
    uint32_t m_type : TypeBits;

    static const uint32_t MaxIndex = (1 << StorageBits) - 1;
    static const uint32_t MaxType  = (1 << TypeBits) - 1;
};

template <size_t I, size_t G, size_t T>
inline uint32_t TypedHandle<I, G, T>::index() const
{
    return _index;
}

template <size_t I, size_t G, size_t T>
inline uint32_t TypedHandle<I, G, T>::generation() const
{
    return _generation;
}

template <size_t I, size_t G, size_t T>
inline uint32_t TypedHandle<I, G, T>::type() const
{
    return m_type;
}

template <size_t I, size_t G, size_t T>
inline void TypedHandle<I, G, T>::setIndex(uint32_t index)
{
    assert(index <= MaxIndex);
    _index = index;
}

template <size_t I, size_t G, size_t T>
inline void TypedHandle<I, G, T>::setGeneration(uint32_t generation)
{
    // generation can wrap-around we're not worried about this
    _generation = generation;
}

template <size_t I, size_t G, size_t T>
inline void TypedHandle<I, G, T>::setType(uint32_t type)
{
    assert(type <= MaxType);
    m_type = type;
}

#pragma pack(pop)
}  // atlas namespace
