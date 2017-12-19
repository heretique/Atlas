#pragma once

#include <functional>

namespace atlas
{
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T1>
inline size_t make_hash(T1 t1)
{
    return std::hash<T1>(t1);
}

template <typename T1, typename T2>
inline size_t make_hash(T1 t1, T2 t2)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    return seed;
}

template <typename T1, typename T2, typename T3>
inline size_t make_hash(T1 t1, T2 t2, T3 t3)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    hash_combine(seed, std::hash<T3>()(t3));
    return seed;
}

template <typename T1, typename T2, typename T3, typename T4>
inline size_t make_hash(T1 t1, T2 t2, T3 t3, T4 t4)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    hash_combine(seed, std::hash<T3>()(t3));
    hash_combine(seed, std::hash<T4>()(t4));
    return seed;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline size_t make_hash(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    hash_combine(seed, std::hash<T3>()(t3));
    hash_combine(seed, std::hash<T4>()(t4));
    hash_combine(seed, std::hash<T5>()(t5));
    return seed;
}
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline size_t make_hash(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    hash_combine(seed, std::hash<T3>()(t3));
    hash_combine(seed, std::hash<T4>()(t4));
    hash_combine(seed, std::hash<T5>()(t5));
    hash_combine(seed, std::hash<T6>()(t6));
    return seed;
}
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline size_t make_hash(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
{
    size_t seed = std::hash<T1>()(t1);
    hash_combine(seed, std::hash<T2>()(t2));
    hash_combine(seed, std::hash<T3>()(t3));
    hash_combine(seed, std::hash<T4>()(t4));
    hash_combine(seed, std::hash<T5>()(t5));
    hash_combine(seed, std::hash<T6>()(t6));
    hash_combine(seed, std::hash<T7>()(t7));
    return seed;
}
}
