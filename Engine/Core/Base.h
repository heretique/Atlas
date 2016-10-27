#ifndef BASE_H
#define BASE_H

#include <new>
#include <memory>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <locale>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <atomic>
#include <iostream>

// Bring common functions from C++ into global namespace
using std::memcpy;
using std::fabs;
using std::sqrt;
using std::cos;
using std::sin;
using std::tan;
using std::isspace;
using std::isdigit;
using std::toupper;
using std::tolower;
using std::size_t;
using std::min;
using std::max;
using std::modf;
using std::atoi;
using std::string;
using std::vector;
using std::map;
using std::thread;
using std::atomic;
using std::atomic_flag;
using std::cout;
using std::endl;


// Types
typedef unsigned int uint;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uintptr_t uptr;

// make_unique
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

// make_resource
template< class T >
using decay_t = typename std::decay<T>::type;

template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args) ->
decltype(std::unique_ptr<decay_t<decltype(*c(std::forward<Arguments>(args)...))>, decltype(d)>(c(std::forward<Arguments>(args)...), d))
{
    auto r = c(std::forward<Arguments>(args)...);
    if (!r) { throw std::system_error(errno, std::generic_category()); }
    return std::unique_ptr<decay_t<decltype(*r)>, decltype(d)>(r, d);
}

// Current function macro.
#ifdef WIN32
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
#endif

// Assert macros.
#ifdef _DEBUG
#include <cassert>
#define ASSERT(expression) assert(expression)
#else
#define ASSERT(expression)
#endif

#if defined(WIN32) && defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif

// Error macro.
#ifdef _ERRORS_AS_WARNINGS
#define _ERROR _WARN
#else
#define _ERROR(...) do \
    { \
    \
    } while (0)
#endif

// Warning macro.
#define _WARN(...) do \
    { \
    \
    } while (0)


// Math
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#ifndef M_1_PI
#define M_1_PI                      0.31830988618379067154
#endif


// NOMINMAX makes sure that windef.h doesn't add macros min and max
#ifdef WIN32
    #define NOMINMAX
#endif

#endif
