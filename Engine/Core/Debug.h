#pragma once

#include <cassert>

// Current function macro.
#ifdef WIN32
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
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
#define _ERROR(...) \
    do              \
    {               \
    } while (0)
#endif

// Warning macro.
#define _WARN(...) \
    do             \
    {              \
    } while (0)
